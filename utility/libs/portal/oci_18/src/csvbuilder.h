#ifndef _csvbuilderH_
#define _csvbuilderH_
#include "databuilder2.h"
#include "xstring.h"
#include "tbuffer.h"
#include "splits.h"

class CsvDataBuilder : public DataBuilder
{
protected:
    Splits *splits;
    char delim;
    char *recname;
    bool useInputs;
    TBChar header;
    TBChar make;
    int used;
public:
    void setUseInputs(bool enablement)
    {
        useInputs = enablement;
    }
    CsvDataBuilder(char delim = ',')
    {
        splits = 0;
        this->delim = delim;
        recname = 0;
        useInputs = false;
        make.sizeto(4096);
        used = 0;
    }
    virtual ~CsvDataBuilder()
    {
        if (splits)
            splits_done(&splits);
        if (recname)
            delete[] recname;
        splits = 0;
        recname = 0;
    }
    void count(int value)
    {
        splits_init(&splits, value);
    }
    void name(const char *inName)
    {
        recname = new char[strlen(inName) + 1];
        strcpy(recname, inName);
    }
    char *getHeader()
    {
        return header.data;
    }
    char *getMake()
    {
        used = 0;
        if (make.used == 0)
        {
            splits_make(splits, make.data, make.size, delim);
            return make.data;
        }
        return "";
    }
    void setHeader(const char *name)
    {
        if (header.used != 0)
            header.append(delim);
        header.append(name);
    }
    void add(const char *name, int8    value, const char *io)
    {
        add(name, (int32)value, io);
    }
    void add(const char *name, int16   value, const char *io)
    {
        add(name, (int32)value, io);
    }
    void add(const char *name, int32   value, const char *io)
    {
        if (io[0] == 'i' && useInputs == false) return;
        setHeader(name);
        splits_asi(splits, (short)value, used++);
    }
    void add(const char *name, int64   value, const char *io)
    {
        if (io[0] == 'i' && useInputs == false) return;
        setHeader(name);
        splits_asl(splits, value, used++);
    }
    void add(const char *name, char   *value, size_t size, const char *io)
    {
        if (io[0] == 'i' && useInputs == false) return;
        setHeader(name);
        char work[4096];
        strncpyz(work, value, size);
        splits_asch(splits, strtrim(work), used++);
    }
    void add(const char *name, double  value, const char *io)
    {
        if (io[0] == 'i' && useInputs == false) return;
        setHeader(name);
        splits_asf(splits, value, used++);
    }
    void add(const char *name, void *value, size_t size, const char *io)
    {
        if (io[0] == 'i' && useInputs == false) return;
        setHeader(name);
        splits_asch(splits, "<blob>", used++);
    }
    void add(const char *name, tHugeCHAR &value, const char *io)
    {
        if (io[0] == 'i' && useInputs == false) return;
        setHeader(name);
        splits_asch(splits, "<clob>", used++);
    }
    void set(const char *name, int8   &value, size_t size, const char *io) {}
    void set(const char *name, int16  &value, size_t size, const char *io) {}
    void set(const char *name, int32  &value, size_t size, const char *io) {}
    void set(const char *name, int64  &value, size_t size, const char *io) {}
    void set(const char *name, char   *value, size_t size, const char *io) {}
    void set(const char *name, double &value, size_t size, const char *io) {}
    void set(const char *name, void   *value, size_t size, const char *io) {}
    void set(const char *name, tHugeCHAR &value, size_t size, const char *io) {}
};

class MDDataBuilder : public CsvDataBuilder
{
protected:
    TBChar bar;
public:
    MDDataBuilder(char delim = '|')
        : CsvDataBuilder('|')
    {
    }
    virtual ~MDDataBuilder()
    {
    }
    char *getBar()
    {
        bar.set(header.data);
        for (size_t i = 0; i < bar.used; i++)
        {
            if (bar.data[i] != delim && bar.data[i] != 0)
                bar.data[i] = '-';
        }
        return bar.data;
    }

};

/**
useful for data from idl2 file calls
will remove memory for recs array on destruction
example of usage:
  ...
  autoCsvRecs<tReportParameter> rp;
  server.ReportParameterSelectAll(&rp.noOf, rp.recs, 0);
  rp.asCSV(stdout);
  rp.asMD("reportparameter.md");
  ...
*/
template <class T>
struct autoCsvRecs
{
    int32 noOf;
    T *recs;
    autoCsvRecs()
    {
        noOf = 0;
        recs = 0;
    }
    ~autoCsvRecs()
    {
        if (recs) free(recs);
        noOf = 0;
        recs = 0;
    }
    void asCSV(FILE *ofile)
    {
        for (int32 i = 0; i < noOf; i++)
        {
            T &rec = recs[i];
            CsvDataBuilder csv;
            rec.BuildData(csv);
            if (i == 0)
                fprintf(ofile, "%s\n", csv.getHeader());
            fprintf(ofile, "%s\n", csv.getMake());
        }
    }
    void asCSV(const char *filename)
    {
        FILE *ofile = fopen(filename, "wt");
        if (ofile == 0) return;
        asCSV(ofile);
        fclose(ofile);
    }
    void asMD(FILE *ofile)
    {
        for (int32 i = 0; i < noOf; i++)
        {
            T &rec = recs[i];
            MDDataBuilder md;
            rec.BuildData(md);
            if (i == 0)
            {
                fprintf(ofile, "%s\n", md.getHeader());
                fprintf(ofile, "%s\n", md.getBar());
            }
            fprintf(ofile, "%s\n", md.getMake());
        }
    }
    void asMD(const char *filename)
    {
        FILE *ofile = fopen(filename, "wt");
        if (ofile == 0) return;
        asMD(ofile);
        fclose(ofile);
    }
};


#endif
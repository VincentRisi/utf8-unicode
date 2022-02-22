
#pragma nosequence

/************************************************************************/
/*                                                                      */
/*                       Z D S E N D . H                                */
/*                                                                      */
/*     Version 94/01/24 (JV)                                            */
/*                                                                      */
/************************************************************************/

#ifndef ZDCIDSND_DEFINED
#define ZDCIDSND_DEFINED "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(ZDCIDSND_DEFINED);

#ifdef ASCII
                                       /*  ASCII   */
#define    CR                 0x0D
#define    DC1                0x11
#define    DC2                0x12
#define    DC3                0x13
#define    DC4                0x14
#define    EM                 0x19
#define    ESC                0x1B
#define    ETX                0x03
#define    FF                 0x0C

#ifdef TEST_DISPLAY
  #define    FS                 '('
  #define    LF                 0x0A
  #define    GS                 '{'
  #define    NUL                0x00
  #define    RS                 ']'
  #define    TAB                0x09
  #define    US                 '['
#else
  #define    FS                 0x1C
  #define    LF                 0x0A
  #define    GS                 0x1D
  #define    NUL                0x00
  #define    RS                 0x1E
  #define    TAB                0x09
  #define    US                 0x1F
#endif

#else
                                       /*  EBCDIC  */
#define    CR                 0x0D
#define    DC1                0x11
#define    DC2                0x12
#define    DC3                0x13
#define    DC4                0x3C
#define    EM                 0x19
#define    ESC                0x27
#define    ETX                0x03
#define    FF                 0x0C
#define    FS                 0x1C
#define    LF                 0x25
#define    GS                 0x1D
#define    NUL                0x00
#define    RS                 0x1E
#define    TAB                0x05
#define    US                 0x1F

#endif

#define    CRLF               CR, LF
#define    CLRH               ESC, 'X', FF
#define    LOCAL              ESC, '='
#define    BIG                ESC, '<'

/****************/
/* device types */
/****************/

#define DEVHPLASER        2
#define DEVTRANSPARRENT   3
#define DEVETTERM1        4
#define DEVCQENCODER      5
#define DEVFAX            6
#define DEVETTERM2        7
#define DEVADDRPRN        9
#define DEVATM           10
#define DEVCATTERM       12
#define DEVTELEX         14
#define DEVETTERM3       18

/****************/
/** zdsend_msg **/
/****************/

#define ZD_HEADER_SIZE                     400
#define ZD_DATA_SIZE                       2000
#define ZD_MESSAGE_SIZE                    2400

/* Header type values */
#define OLD_V_HDR        "01"
#define CONVERSN_HDR     "02"

/* Network messages types */
#define NWDATAMSG                       "01"
#define NWNCMSG                         "03"
#define NWCONFMSG                       "04"
#define NWFLOWMSG               "05"
#define NWSTATMSG                       "06"
#define NWCORPMSG               "09"
#define NWP2PMSG                "10"
#define NWP2PSMSG                       "11"

/* pgmtype values */
#define PGMTYPE_EOT                     'A'
#define PGMTYPE_NORMAL      'B'
#define PGMTYPE_P2P         'C'
#define PGMTYPE_CORP        'D'
#define PGMTYPE_FLOW        'E'
#define PGMTYPE_QDEPTH      'F'
#define PGMTYPE_P2PS        'G'

/* P2P routing codes */
#define P2P_PGM_GENERAL         "999"
#define P2P_PGM_NEDBANK     "998"
#define P2P_PGM_NEDFIN      "997"
#define P2P_PGM_UAL         "996"

#define P2P_GENERAL                     999
#define P2P_NEDBANK             998
#define P2P_NEDFIN              997
#define P2P_UAL                 996

struct ZDSEND_MESSAGE_HEADER {
  char                         header_type       [2]; /* DISP NUMERIC  */
  char                         program           [8]; /*               */
  char                         filler_1          [6];
  char                         head_pgmtype         ; /*               */
  char                         filler_2             ;
  char                         origin_dev_v      [2]; /* DISP NUMERIC  */
  char                         terminal          [4]; /* DISP NUMERIC  */
  char                         origin_term_v     [4]; /* DISP NUMERIC  */
  char                         message_size      [4]; /* DISP NUMERIC  */
  char                         device_type       [2]; /* DISP NUMERIC  */
  char                         time_bct             ; /* DISP NUMERIC  */
  char                         term_coy          [2]; /* DISP NUMERIC  */
  char                         int_fax_tlx          ; /* DISP NUMERIC  */
  char                         sec_action           ; /* DISP NUMERIC  */
  char                         time              [5]; /* DISP NUMERIC  */
  char                         time_milli        [5]; /* DISP NUMERIC  */
  char                         program_number    [3]; /* DISP NUMERIC  */
  char                         msg_type          [2]; /* DISP NUMERIC  */
  char                         action            [2]; /* DISP NUMERIC  */
  char                         return_cde        [5]; /* DISP NUMERIC  */
  char                         src_netno         [5]; /* DISP NUMERIC  */
  char                         src_lsn           [5]; /* DISP NUMERIC  */
  char                         src_dev           [2]; /* DISP NUMERIC  */
  char                         src_corp          [5]; /* DISP NUMERIC  */
  char                         src_term          [5]; /* DISP NUMERIC  */
  char                         origin_term       [5]; /* DISP NUMERIC  */
  char                         origin_dev        [2]; /* DISP NUMERIC  */
  char                         dst_netno         [5]; /* DISP NUMERIC  */
  char                         dst_lsn           [5]; /* DISP NUMERIC  */
  char                         dst_dev           [2]; /* DISP NUMERIC  */
  char                         dst_corp          [5]; /* DISP NUMERIC  */
  char                         dst_term          [5]; /* DISP NUMERIC  */
  char                         term_branch       [5]; /* DISP NUMERIC  */
  char                         mini_branch       [5]; /* DISP NUMERIC  */
  char                         waste_ind            ; /*               */
  char                         echo_data        [20]; /*               */
  char                         echo_sess_no      [2]; /* DISP NUMERIC  */
  char                         echo_cc_msg          ; /*               */
  char                         echo_cc_ss        [4]; /* DISP NUMERIC  */
  char                         echo_cc_xctl         ; /*               */
  char                         cc_pgm            [3]; /* DISP NUMERIC  */
  char                         sec_emp_1         [6]; /* DISP NUMERIC  */
  char                         sec_logon_1          ; /* DISP NUMERIC  */
  char                         sec_pword_1          ; /* DISP NUMERIC  */
  char                         sec_level_1       [2]; /* DISP NUMERIC  */
  char                         sec_coy_1         [2]; /* DISP NUMERIC  */
  char                         sec_branch_1      [5]; /* DISP NUMERIC  */
  char                         sec_emp_2         [6]; /* DISP NUMERIC  */
  char                         sec_logon_2          ; /* DISP NUMERIC  */
  char                         sec_pword_2          ; /* DISP NUMERIC  */
  char                         sec_level_2       [2]; /* DISP NUMERIC  */
  char                         sec_coy_2         [2]; /* DISP NUMERIC  */
  char                         sec_branch_2      [5]; /* DISP NUMERIC  */
  char                         dbug_ind             ; /* DISP NUMERIC  */
  char                         dst_trx_window    [5]; /* DISP NUMERIC  */
  char                         int_netno         [5]; /* DISP NUMERIC  */
  char                         int_lsn           [5]; /* DISP NUMERIC  */
  char                         filler_3        [196]; /*               */
  };


struct ZDSEND_MESSAGE {
  struct ipc_header            ipchdr;
  struct ZDSEND_MESSAGE_HEADER hdr;
  char                         data             [ZD_DATA_SIZE];
  };

struct ZDSEND_CMESSAGE {
  struct ipc_cheader           ipchdr;
  struct ZDSEND_MESSAGE_HEADER hdr;
  char                         data             [ZD_DATA_SIZE];
  };


struct ZDSEND_DATAGR_HEADER {
  char                         prefix            [1]; /*               */
  char                         contr             [1]; /* DISP NUMERIC  */
  char                         screen            [6]; /*               */
  char                         block             [1]; /* DISP NUMERIC  */
  char                         pres              [2]; /* DISP NUMERIC  */
  char                         phase             [6]; /* DISP NUMERIC  */
  char                         error             [3]; /* DISP NUMERIC  */
  char                         data_len          [4]; /* DISP NUMERIC  */
  char                         filler_1          [6]; /*               */
  };

struct ZDSENDUSERPARAMETERS {
  char                         verify1           [6]; /*  ZDSEND       */
  char                         unix_parms       [18]; /*               */
  struct boj_area              boj_fields;            /*               */
  char                         mcs_id            [8]; /*               */
  char                         filler_1          [6];
  char                         header_type       [2]; /* DISP NUMERIC  */
  char                         it_now               ; /* DISP NUMERIC  */
  char                         buffer_count      [4]; /* DISP NUMERIC  */
  char                         return_cd            ; /* DISP NUMERIC  */
  char                         function             ; /*               */
  char                         msg_type             ; /*               */
  char                         form_data            ; /* DISP NUMERIC  */
  char                         form_number       [4]; /* DISP NUMERIC  */
  char                         form_pack         [6]; /*               */
  char                         fax_implmentd        ; /* DISP NUMERIC  */
  char                         abort_fax            ; /* DISP NUMERIC  */
  char                         abort_fax_msg    [80]; /*               */
  char                         fax_headers      [160];/*               */
  char                         dc               [10]; /* DISP NUMERIC  */
  struct ZDSEND_DATAGR_HEADER datagr_head;
  char                         filler_2         [100];/*               */
  };

struct ZDLNLG_DATA {
  char                         nelg              [4]; /* NELG          */
  char                         text             [38]; /*               */
  char                         type                 ; /*               */
  char                         id_numeric        [6]; /* DISP NUMERIC  */
  char                         display              ; /* DISP NUMERIC  */
  char                         terminal          [4]; /* DISP NUMERIC  */
  char                         device            [2]; /* DISP NUMERIC  */
};

struct ZDLNLG_MESSAGE {
  struct ipc_header            ipchdr;
  struct ZDSEND_MESSAGE_HEADER hdr;
  struct ZDLNLG_DATA           data;
};

/*********************************************************************/
/**                       ZDFRMU.DAT                                **/
/*********************************************************************/

#define Move_Pointer_File_Error                 -2
#define Read_File_Error                         -3
#define End_Of_File_Error                       -4
#define Write_File_Error                        -5
#define No_RAM_File_Error                       -6
#define Create_File_Error                       -7
#define Open_File_Error                         -8
#define Close_File_Error                        -9
#define Read_Only                               1
#define Update                                  2
#define Create                                  3
#define Append                                  4
/*************** Forms record layout *********************************/

struct ZDFRMU_RECORD
{
    char         Form        [1500];
    char         Etx             ;
    char         System_Id   [6];
    char         Description [20];
    char         Form_Number [4];
    char         Last_Active [6];
    char         Locked      ;
    char         Next_Avail  [4];
    char         Form_Size   [4];
    char         Last_Unlock [5];
    char         Applic_Id   [6];
    char         Io_Cnt_Curr [5];
    char         Io_Cnt_Prev [5];
        char             Unlock_Date [5];
    char         Filler      [48];
};

#ifdef BITMAP_L_R
  struct CONTROL
  {
    unsigned   act   : 4;
    unsigned   fil   : 2;
    unsigned   nod   : 1;
    unsigned   nol   : 1;
  };
#else
  struct CONTROL
  {
    unsigned   nol   : 1;
    unsigned   nod   : 1;
    unsigned   fil   : 2;
    unsigned   act   : 4;
  };
#endif

/*********************************************************************/
/*                                                                   */
/*     functions  (procedures) in zpsend                             */
/*                                                                   */
/*********************************************************************/

int ZPSENDBOJ    (struct ZDSENDUSERPARAMETERS *p);
int ZPSENDFROMCS (struct ZDSEND_MESSAGE *pMsg);
int ZPSEND       (struct ZDSEND_MESSAGE *pMsg, char *pLine);
int ZPCDEV       (void);
int ZPFORM       (struct ZDSEND_MESSAGE *pMsg, char *pLine, char *pFdata);
int ZPFORM_OPEN  (int mode);
int ZPFORM_READ  (char Form_No [4], struct ZDFRMU_RECORD *pFbuf);
int ZPFORM_WRITE (char Form_No [4], struct ZDFRMU_RECORD *pFbuf);
int ZPFORM_PARSE (char Form_No [4], struct ZDFRMU_RECORD *pFbuf,
                                    struct ZDSEND_MESSAGE *pMsg);

int INCDSP (char *display, int increment, int size);
void ITOA  (char *display, int integer, int size);

/************************************************************************/
/**                                                                    **/
/**                       TRANSLATE TABLES                             **/
/**                                                                    **/
/************************************************************************/

static char *Asc_Asc =     /* Translate Ascii - Control chars to space */
/*           0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */

/*                                                                  so  si   */
/* 0 */  "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x20\x20"
/*                                          can em  sub     fs  gs  rs  us   */
/* 1 */  "\x10\x11\x12\x13\x14\x15\x16\x17\x20\x20\x20\x1B\x5B\x5B\x5D\x5B"
/* 2 */  "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
/* 3 */  "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
/* 4 */  "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
/* 5 */  "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\\\x5D\x5E\x5F"
/* 6 */  "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
/* 7 */  "\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"

/* 8 */  "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
/* 9 */  "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
/* A */  "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
/* B */  "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
/* C */  "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF"
/* D */  "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF"
/* E */  "\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
/* F */  "\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF"

/*           0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */

;


static char *Ebc_Ebc =     /* Translate EBCDIC - Control chars to space */
/*           0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */

/*                                                                  so  si   */
/* 0 */  "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x40\x40"
/*                                          can em          fs  gs  rs  us   */
/* 1 */  "\x10\x11\x12\x13\x14\x15\x16\x17\x40\x40\x1A\x1B\x4A\x4A\x4A\x5A"
/* 2 */  "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
/*                                                                      sub  */
/* 3 */  "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x40"
/* 4 */  "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
/* 5 */  "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\\\x5D\x5E\x5F"
/* 6 */  "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
/* 7 */  "\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"

/* 8 */  "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
/* 9 */  "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
/* A */  "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
/* B */  "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
/* C */  "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF"
/* D */  "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF"
/* E */  "\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
/* F */  "\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF"

/*           0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */

;

/*********************************************************************/
/**                       END OF zdsend.h                           **/
/*********************************************************************/

#endif


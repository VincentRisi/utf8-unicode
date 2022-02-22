## To create a PDB:

1. In SQL*Plus, ensure that the current container is the CDB root or an application root.

    When the current container is the CDB root, the PDB is created in the CDB using the files of the PDB seed.

    When the current container is an application root, the application PDB is created in the application container using the files of the application seed. If there is no application seed in the application container, then the application PDB is created in the application container using the files of the PDB seed.

2. Run the CREATE PLUGGABLE DATABASE statement, and specify a local administrator for the PDB. Specify other clauses when they are required.

    After you create the PDB, it is in mounted mode, and its status is NEW. You can view the open mode of a PDB by querying the OPEN_MODE column in the V$PDBS view. You can view the status of a PDB by querying the STATUS column of the CDB_PDBS or DBA_PDBS view.

    A new default service is created for the PDB. The service has the same name as the PDB and can be used to access the PDB. Oracle Net Services must be configured properly for clients to access this service.

3. Open the new PDB in read/write mode.

    You must open the new PDB in read/write mode for Oracle Database to complete the integration of the new PDB into the CDB. An error is returned if you attempt to open the PDB in read-only mode. After the PDB is opened in read/write mode, its status is NORMAL.

4. Back up the PDB.

    A PDB cannot be recovered unless it is backed up.

``` sql
CREATE PLUGGABLE DATABASE salespdb ADMIN USER salesadm IDENTIFIED BY pwd;

create pluggable database vanguard admin user vince identified by oracle;

CREATE PLUGGABLE DATABASE vandb 
  ADMIN USER vince IDENTIFIED BY oracle
  STORAGE (MAXSIZE 2G)
  DEFAULT TABLESPACE vandb
    DATAFILE 'D:\software\oracle\WINDOWS_X64_193000_db_home\dbs\vandbpdb\vandb01.dbf' SIZE 250M 
    AUTOEXTEND ON
    PATH_PREFIX = 'D:\software\oracle\WINDOWS_X64_193000_db_home\dbs\vandbpdb\'
    FILE_NAME_CONVERT = ('D:\software\oracle\WINDOWS_X64_193000_db_home\dbs\pdbseed\', 
                         'D:\software\oracle\WINDOWS_X64_193000_db_home\dbs\vandbpdb\');

```

``` sql
create user <username> identified by "<password>";
```
``` sql
grant <privilege> to <user>;
grant create session to data_owner;
grant create table to data_owner;

```
- create view – Allows you to create views
- create procedure – Gives the ability to create procedures, functions and packages
- create sequence – The ability to make sequences
- SYSASM
- SYSOPER
- SYSDBA

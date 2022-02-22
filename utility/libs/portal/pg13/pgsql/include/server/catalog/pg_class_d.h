/*-------------------------------------------------------------------------
 *
 * pg_class_d.h
 *    Macro definitions for pg_class
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * NOTES
 *  ******************************
 *  *** DO NOT EDIT THIS FILE! ***
 *  ******************************
 *
 *  It has been GENERATED by src/backend/catalog/genbki.pl
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_CLASS_D_H
#define PG_CLASS_D_H

#define RelationRelationId 1259
#define RelationRelation_Rowtype_Id 83

#define Anum_pg_class_oid 1
#define Anum_pg_class_relname 2
#define Anum_pg_class_relnamespace 3
#define Anum_pg_class_reltype 4
#define Anum_pg_class_reloftype 5
#define Anum_pg_class_relowner 6
#define Anum_pg_class_relam 7
#define Anum_pg_class_relfilenode 8
#define Anum_pg_class_reltablespace 9
#define Anum_pg_class_relpages 10
#define Anum_pg_class_reltuples 11
#define Anum_pg_class_relallvisible 12
#define Anum_pg_class_reltoastrelid 13
#define Anum_pg_class_relhasindex 14
#define Anum_pg_class_relisshared 15
#define Anum_pg_class_relpersistence 16
#define Anum_pg_class_relkind 17
#define Anum_pg_class_relnatts 18
#define Anum_pg_class_relchecks 19
#define Anum_pg_class_relhasrules 20
#define Anum_pg_class_relhastriggers 21
#define Anum_pg_class_relhassubclass 22
#define Anum_pg_class_relrowsecurity 23
#define Anum_pg_class_relforcerowsecurity 24
#define Anum_pg_class_relispopulated 25
#define Anum_pg_class_relreplident 26
#define Anum_pg_class_relispartition 27
#define Anum_pg_class_relrewrite 28
#define Anum_pg_class_relfrozenxid 29
#define Anum_pg_class_relminmxid 30
#define Anum_pg_class_relacl 31
#define Anum_pg_class_reloptions 32
#define Anum_pg_class_relpartbound 33

#define Natts_pg_class 33


#define		  RELKIND_RELATION		  'r'	/* ordinary table */
#define		  RELKIND_INDEX			  'i'	/* secondary index */
#define		  RELKIND_SEQUENCE		  'S'	/* sequence object */
#define		  RELKIND_TOASTVALUE	  't'	/* for out-of-line values */
#define		  RELKIND_VIEW			  'v'	/* view */
#define		  RELKIND_MATVIEW		  'm'	/* materialized view */
#define		  RELKIND_COMPOSITE_TYPE  'c'	/* composite type */
#define		  RELKIND_FOREIGN_TABLE   'f'	/* foreign table */
#define		  RELKIND_PARTITIONED_TABLE 'p' /* partitioned table */
#define		  RELKIND_PARTITIONED_INDEX 'I' /* partitioned index */

#define		  RELPERSISTENCE_PERMANENT	'p' /* regular table */
#define		  RELPERSISTENCE_UNLOGGED	'u' /* unlogged permanent table */
#define		  RELPERSISTENCE_TEMP		't' /* temporary table */

/* default selection for replica identity (primary key or nothing) */
#define		  REPLICA_IDENTITY_DEFAULT	'd'
/* no replica identity is logged for this relation */
#define		  REPLICA_IDENTITY_NOTHING	'n'
/* all columns are logged as replica identity */
#define		  REPLICA_IDENTITY_FULL		'f'
/*
 * an explicitly chosen candidate key's columns are used as replica identity.
 * Note this will still be set if the index has been dropped; in that case it
 * has the same meaning as 'd'.
 */
#define		  REPLICA_IDENTITY_INDEX	'i'

/*
 * Relation kinds that have physical storage. These relations normally have
 * relfilenode set to non-zero, but it can also be zero if the relation is
 * mapped.
 */
#define RELKIND_HAS_STORAGE(relkind) \
	((relkind) == RELKIND_RELATION || \
	 (relkind) == RELKIND_INDEX || \
	 (relkind) == RELKIND_SEQUENCE || \
	 (relkind) == RELKIND_TOASTVALUE || \
	 (relkind) == RELKIND_MATVIEW)



#endif							/* PG_CLASS_D_H */

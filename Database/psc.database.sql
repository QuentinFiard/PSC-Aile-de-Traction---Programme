-- Creator:       MySQL Workbench 5.2.31/ExportSQLite plugin 2009.12.02
-- Author:        Quentin Fiard
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2012-02-11 07:24
-- Created:       2012-02-11 07:01
PRAGMA foreign_keys = OFF;

-- Schema: psc_database
BEGIN;
CREATE TABLE "Record"(
  "startDate" INTEGER NOT NULL,
  "tag" VARCHAR(128)
);
CREATE TABLE "Configuration"(
  "field" VARCHAR(32) NOT NULL,
  "dataType" INTEGER,
  "value" BLOB,
  CONSTRAINT "field_UNIQUE"
    UNIQUE("field")
);
CREATE TABLE "Data"(
  "timeElapsed" INTEGER NOT NULL,
  "sourceID" INTEGER NOT NULL,
  "value" BLOB
);
CREATE TABLE "Source"(
  "name" VARCHAR(128) NOT NULL,
  "dataType" INTEGER NOT NULL,
  "recordId" VARCHAR(45),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
COMMIT;

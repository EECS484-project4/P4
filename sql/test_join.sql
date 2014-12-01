-- Create the Datamations type of relations (note these tuples are smaller than that 
--  speficied in the Datamation benchmark).

CREATE TABLE DA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- load tuples into the DA table
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17234, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (3, 19052, '00003 string record',  3.3);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (4, 24069, '00004 string record',  4.4);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (5, 17188, '00005 string record',  5.5);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (6, 28769, '00006 string record',  6.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (6, 28770, '00006 string record',  6.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (7, 15168, '00007 string record',  7.7);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (8, 578, '00008 string record',  8.8);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (9, 21055, '00009 string record',  9.9);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (10, 7648, '00010 string record', 11.0);


-- load tuples into the DB table
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1440, '00002 string record',  2.2);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1550, '00002 string record',  2.2);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (3, 20441, '00003 string record',  3.3);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (5, 31830, '00005 string record',  5.5);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (6, 18816, '00006 string record',  6.6);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (6, 18817, '00006 string record',  6.6);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (7, 22849, '00007 string record',  7.7);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (8, 28448, '00008 string record',  8.8);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (9, 27189, '00009 string record',  9.9);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (10, 1479, '00010 string record', 11.0);


-- Test selection queries
-- simple nested loop
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial <= DB.serial; 
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial < DB.serial;
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial > DB.serial;
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial >= DB.serial;
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial <> DB.serial;

-- index nested loop
CREATE INDEX DA (serial);
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial = DB.serial;

CREATE INDEX DB (serial);
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial = DB.serial;

DROP INDEX DA (serial);
SELECT DA.serial, DB.serial FROM DA, DB WHERE DA.serial = DB.serial;

DROP INDEX DB (serial);

DROP TABLE DA;

DROP TABLE DB;

-- sort merge join
CREATE TABLE DA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- load tuples into the DA table
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (9, 21055, '00009 string record',  9.9);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (3, 19052, '00003 string record',  3.3);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (8, 578, '00008 string record',  8.8);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (7, 15168, '00007 string record',  7.7);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17234, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (6, 28769, '00006 string record',  6.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (6, 28770, '00006 string record',  6.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (5, 17188, '00005 string record',  5.5);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (10, 7648, '00010 string record', 11.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (4, 24069, '00004 string record',  4.4);


-- load tuples into the DB table
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1440, '00002 string record',  2.2);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (3, 20441, '00003 string record',  3.3);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (6, 18816, '00006 string record',  6.6);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (9, 27189, '00009 string record',  9.9);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (6, 18817, '00006 string record',  6.6);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1550, '00002 string record',  2.2);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (7, 22849, '00007 string record',  7.7);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (8, 28448, '00008 string record',  8.8);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (10, 1479, '00010 string record', 11.0);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (5, 31830, '00005 string record',  5.5);

SELECT DA.serial, DB.serial, DA.ikey, DB.ikey FROM DA, DB WHERE DA.serial = DB.serial; -- use SMJ

DROP TABLE DA;
DROP TABLE DB;
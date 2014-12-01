
-- Create the Datamations type of relations (note these tuples are smaller than that 
--  speficied in the Datamation benchmark).

CREATE TABLE DA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- load 10 tuples into the DA table
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (3, 19052, '00003 string record',  3.3);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (4, 24069, '00004 string record',  4.4);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (5, 17188, '00005 string record',  5.5);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (6, 28769, '00006 string record',  6.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (7, 15168, '00007 string record',  7.7);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (8, 578, '00008 string record',  8.8);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (9, 21055, '00009 string record',  9.9);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (10, 7648, '00010 string record', 11.0);

-- Test selection queries
-- simple file scan
SELECT * FROM DA WHERE DA.serial = 5; 

SELECT * FROM DA WHERE DA.serial <> 5;

SELECT * FROM DA WHERE DA.serial < 5;

SELECT * FROM DA WHERE DA.serial <= 5;

SELECT * FROM DA WHERE DA.serial > 5;

SELECT * FROM DA WHERE DA.serial >= 5;

SELECT * FROM DA WHERE DA.filler = '00005 string record';

SELECT * FROM DA;

SELECT DA.serial, DA.ikey FROM DA WHERE DA.serial = 5;

-- index select
CREATE INDEX DA (serial);
SELECT * FROM DA WHERE DA.serial = 5;  -- use index select

DROP INDEX DA (serial);

DROP TABLE DA;
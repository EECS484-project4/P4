-- Test selection queries

CREATE INDEX DA (serial);
SELECT * FROM DA, DB WHERE DA.serial = DB.serial; -- use INL
--CREATE INDEX DA (ikey);
--SELECT * FROM DA, DB WHERE DA.ikey = DB.ikey; -- use INL

--DROP INDEX DA (ikey);
--SELECT * FROM DA, DB WHERE DA.ikey = DB.ikey; -- use SMJ

--SELECT DA.ikey, DB.serial FROM DA, DB WHERE DA.ikey <= DB.serial; -- use SNL


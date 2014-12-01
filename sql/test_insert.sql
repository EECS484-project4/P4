--
-- test Insert with and without indices
--

-- create relations
CREATE TABLE soaps(soapid integer, name char(32), 
                   network char(4), rating double);
CREATE TABLE stars(starid integer, real_name char(20), 
                   plays char(12), soapid integer);

-- insert with duplicate attributes 
INSERT INTO stars (real_name, real_name, starid, plays) 
	VALUES ('George, David', 'David, David', 101, 'Cassie');

-- insert with missing attrivutes 
INSERT INTO stars (real_name, soapid, starid) 
	VALUES ('Parker, Tony', 5, 106);

-- insert with too many attributes 
INSERT INTO stars (real_name, soapid, starid, starid, plays) 
	VALUES ('Bonarrigo, Laura', 10, 100, 90,'Cassie');

-- insert with attributes with invalid type
INSERT INTO stars (real_name, soapid, starid, starid, plays) 
	VALUES (0, 10, 100, 90,'Cassie');

SELECT * FROM stars;

DROP TABLE soaps;
DROP TABLE stars;

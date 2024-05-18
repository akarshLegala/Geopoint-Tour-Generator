#ifndef geodb_h
#define geodb_h
#include "geopoint.h"
#include "base_classes.h"
#include "geotools.h"
#include <vector>
#include <string>
#include "hashmap.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//1. Load all of the OSM map data from a text data file into one or more data structures of
//7
//your choosing.
//2. Given a latitude/longitude coordinate c (we call this a GeoPoint), return all
//latitude/longitude coordinates (GeoPoints) that are at the other endpoint of street
//segments that GeoPoint c is one endpoint of, according to the OSM map file.
//3. Given a point of interest (e.g., Diddy Riese), return the GeoPoint where that point of
//interest is located.
//4. Given two GeoPoints (which are directly connected by a segment in the OSM map data),
//return the street name associated with that segment of road (e.g., Westwood Blvd).

class GeoDatabase: public GeoDatabaseBase
{
    public:
     GeoDatabase();
     virtual ~GeoDatabase();
     virtual bool load(const std::string& map_data_file);
     virtual bool get_poi_location(const std::string& poi,
     GeoPoint& point) const;
     virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
     const;
     virtual std::string get_street_name(const GeoPoint& pt1,
     const GeoPoint& pt2) const;
private:

    HashMap<GeoPoint> m_pointOfinterests; //points of interest name to geopoint
    HashMap<string> geopointsToNames; //pts to keys is GeoPoint to vector<string> representing the street names
    HashMap<vector<GeoPoint>> connectedPoints;
    
};




#endif /* geodb_h */

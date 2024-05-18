//
//  geodb.cpp
//  proj 4
//
//  Created by Akarsh Legala on 3/15/24.
//

#include <stdio.h>
#include "geodb.h"
GeoDatabase::GeoDatabase(){}
GeoDatabase::~GeoDatabase(){}


bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
    //map of string to gp so returns nullptr or pointer to value (gp)
    auto ptr = m_pointOfinterests.find(poi);
    if (ptr != nullptr)
    {
        point = *ptr;
        return true;
    }
    return false;
}
// This method takes in the name of a point of interest, such as Ackerman Union, and then
//returns the GeoPoint location of that PoI. It returns true if the PoI was found, and sets its
//second parameter to the GeoPoint of the PoI. Otherwise, it must return false and make no
//changes to the second GeoPoint parameter. This function must run in O(1) (i.e., constant)
//time. Here's an example of its usage:



std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    vector<GeoPoint> tempV;
    auto ptr = connectedPoints.find(pt.to_string());
    if (ptr != nullptr)
    {
        //should create copy cuz not auto&
        return *ptr;
    }
    return tempV;
}
//    This method takes in a GeoPoint and returns a vector of all GeoPoints that are directly
//    connected to that point via street segment(s). If the function finds no points connected to the
//    searched-for point, then the function returns an empty vector. This function must run in O(p)
//    time where p is the number of GeoPoints that are directly connected to the searched-for point.
//    Here's an example of its usage:
std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    string streetName = "";
    const auto* ptr1 = geopointsToNames.find(pt1.to_string() + pt2.to_string());
    
    if (ptr1 != nullptr)
    {
        streetName = *ptr1;
    }
    else
    {
        const auto* ptr2 = geopointsToNames.find(pt2.to_string() + pt1.to_string());
        if (ptr2 != nullptr)
        {
            streetName = *ptr2;
        }
    }
    
    return streetName;
}
bool GeoDatabase::load(const std::string& map_data_file)
{
    ifstream infile(map_data_file);
    if (!infile)
    {
           cerr << "Error opening the file." << endl;
           return false;
    }
    string line;
    while (getline(infile, line)) {
        string streetName = line;
        getline(infile, line);
        istringstream ss(line);
        string startLat, startLon, endLat, endLon;
        ss >> startLat >> startLon >> endLat >> endLon;
        GeoPoint start = GeoPoint(startLat, startLon);
        GeoPoint end = GeoPoint(endLat, endLon);
        
        geopointsToNames.insert(start.to_string() + end.to_string(), streetName);
        
        auto ptr = connectedPoints.find(start.to_string());
        //if start doesnt exist, create it else just push back
        if (ptr == nullptr)
        {
            connectedPoints.insert(start.to_string(), vector<GeoPoint>{end});
        }
        else
        {
            ptr->push_back(end);
        }
        ptr = connectedPoints.find(end.to_string());
        //if end doesnt exist, create it else just push back
        if (ptr == nullptr)
        {
            connectedPoints.insert(end.to_string(), vector<GeoPoint>{start});
        }
        else
        {
            ptr->push_back(start);
        }

        // Read number of points of interest
        getline(infile, line);
        int numPOIs = stoi(line);
        

        if (numPOIs > 0){
            GeoPoint middy = midpoint(start, end);
            //add end and start to middy's vector
            connectedPoints.insert(middy.to_string(), vector<GeoPoint>{end});
            ptr = connectedPoints.find(middy.to_string());
            ptr->push_back(start);
            
            //add middy to start's vector
            ptr = connectedPoints.find(start.to_string());
            ptr->push_back(middy);
            
            //add middy to end's vector
            ptr = connectedPoints.find(end.to_string());
            ptr->push_back(middy);
            
            geopointsToNames.insert(middy.to_string() + start.to_string(), streetName);
            geopointsToNames.insert(middy.to_string() + end.to_string(), streetName);
            for (int i = 0; i < numPOIs; ++i)
            {
                getline(infile, line);
                string poiName = line.substr(0, line.find('|'));
                line = line.substr(line.find('|') +1);
                istringstream iss(line);
                string poiLat, poiLon;
                iss >> poiLat >> poiLon;

                //get POIs location and put it in POIs
                GeoPoint poiLoc = GeoPoint(poiLat, poiLon);
                m_pointOfinterests.insert(poiName, poiLoc);
                //idk if this line is correct or not
                //add midpoint to connected points with poi's name
                connectedPoints.insert(poiLoc.to_string(), vector<GeoPoint> {middy});
                
                //insert like told
                geopointsToNames.insert(middy.to_string() + poiLoc.to_string(), "a path");
                //add POI's location to middy's vector
                ptr = connectedPoints.find(middy.to_string());
                ptr->push_back(poiLoc);
            }
        }
    }


    infile.close();
    return true;
}

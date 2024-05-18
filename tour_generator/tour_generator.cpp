//
//  tour_generator.cpp
//  proj 4
//
//  Created by Akarsh Legala on 3/15/24.
//

#include "tour_generator.h"
#include "geotools.h"
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
    : geodb_(geodb), router_(router) {}

TourGenerator::~TourGenerator() {}

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    vector<TourCommand> tour_commands;

    for (int i = 0; i < stops.size(); ++i) {
        string stop_name, stop_description;
        if (stops.get_poi_data(i, stop_name, stop_description)) {
            // Generate commentary command
            TourCommand commentary_command;
            commentary_command.init_commentary(stop_name, stop_description);
            tour_commands.push_back(commentary_command);

            if (i < stops.size() - 1) {
                // Calculate route to the next stop and generate route commands
                string next_stop_name, next_stop_description;
                stops.get_poi_data(i + 1, next_stop_name, next_stop_description);

                GeoPoint start_point, end_point;
                if (geodb_.get_poi_location(stop_name, start_point) && geodb_.get_poi_location(next_stop_name, end_point)) {
                    auto route = router_.route(start_point, end_point);
                    
                    for (size_t j = 0; j < route.size() - 1; ++j) {
                        const auto& start = route[j];
                        const auto& end = route[j + 1];

                        string street_name = geodb_.get_street_name(start, end);
                        double distance = distance_earth_miles(start, end);
                        double angle = angle_of_line(start, end);

                        string direction = getDirection(angle);

                        // Generate proceed command
                        TourCommand proceed_command;
                        proceed_command.init_proceed(direction, street_name, distance, start, end);
                        tour_commands.push_back(proceed_command);

                        // If there's a next point, possibly generate a turn command
                        if (j < route.size() - 2) {
                            const auto& next_point = route[j + 2];
                            double turn_angle = angle_of_turn(start, end, next_point);
                            if (turn_angle >= 1.0 && turn_angle <= 359.0) {
                                string turn_direction = getTurnDirection(turn_angle);
                                string next_street_name = geodb_.get_street_name(end, next_point);

                                if (street_name != next_street_name) {
                                    TourCommand turn_command;
                                    turn_command.init_turn(turn_direction, next_street_name);
                                    tour_commands.push_back(turn_command);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return tour_commands;
}

string TourGenerator::getDirection(double angle) const {
    if (angle >= 0 && angle < 22.5) {
        return "east";
    } else if (angle >= 22.5 && angle < 67.5) {
        return "northeast";
    } else if (angle >= 67.5 && angle < 112.5) {
        return "north";
    } else if (angle >= 112.5 && angle < 157.5) {
        return "northwest";
    } else if (angle >= 157.5 && angle < 202.5) {
        return "west";
    } else if (angle >= 202.5 && angle < 247.5) {
        return "southwest";
    } else if (angle >= 247.5 && angle < 292.5) {
        return "south";
    } else if (angle >= 292.5 && angle < 337.5) {
        return "southeast";
    } else { // angle >= 337.5
        return "east";
    }
}

string TourGenerator::getTurnDirection(double angle) const {
    if (angle >= 1 and angle < 180) {
        return "left";
    }
    else {
        return "right";
    }
}

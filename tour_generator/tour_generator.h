//
//  tour_generator.h
//  proj 4
//
//  Created by Akarsh Legala on 3/11/24.
//

#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"
#include "stops.h"
#include <vector>

class TourGenerator : public TourGeneratorBase {
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();

    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

private:
    const GeoDatabaseBase& geodb_;
    const RouterBase& router_;

    std::vector<TourCommand> generateCommentaryCommands(const Stops& stops) const;
    std::vector<TourCommand> generateRouteCommands(const std::vector<GeoPoint>& route, const GeoDatabaseBase& geodb) const;
    std::string getDirection(double angle) const;
    std::string getTurnDirection(double angle) const ;
};

#endif /* tour_generator_h */

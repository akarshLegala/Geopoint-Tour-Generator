//
//  router.h
//  proj 4
//
//  Created by Akarsh Legala on 3/11/24.
//

#ifndef router_h
#define router_h

#include "base_classes.h"
#include "geodb.h"
#include "geotools.h"
#include <queue>
#include <vector>
#include <unordered_map>
using namespace std;

class Router : public RouterBase {
public:
    Router(const GeoDatabaseBase& geo_db);

    virtual ~Router() {}
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const override;
    

private:
    const GeoDatabaseBase& map;

    struct Node {
        GeoPoint point;
        double g_score;
        double f_score;
        Node* parent;

        Node(const GeoPoint& p, double g, double f, Node* par) : point(p), g_score(g), f_score(f), parent(par) {}

        bool operator>(const Node& other) const {
            return f_score > other.f_score;
        }
    };

    class NodeComparator {
    public:
        bool operator()(const Node* a, const Node* b) const {
            return *a > *b;
        }
    };

    double heuristic(const GeoPoint& a, const GeoPoint& b) const {
        return distance_earth_miles(a, b); // Using miles for consistency
    }

    vector<GeoPoint> reconstructPath(Node* currentNode) const {
        vector<GeoPoint> path;
        while (currentNode) {
            path.push_back(currentNode->point);
            currentNode = currentNode->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

#endif /* router_h */

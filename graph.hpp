#include <vector>
#include <tuple>
#include <set>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string.h>

#include "idstore.hpp"

#ifndef GRAPH_H
#define GRAPH_H

// ребра графа
class Edge {
    public:
        id from_city, to_city, transport_type;
        cruiseTime time;
        cruiseFare fare;

    public:
        Edge( )
            : from_city(-1), to_city(-1), transport_type(-1), time(-1), fare(-1) {} 
        
        Edge( std::vector<std::string> &input_data, IDStore &city_ids_store, IDStore &transport_ids_store ) {
            std::string from_city_name = input_data[0],
                        to_city_name = input_data[1],
                        transport_name = input_data[2],
                        cruise_time_str = input_data[3],
                        cruise_fare_str = input_data[4];

            // добавление новых id если еще не добавлялись
            if ( !city_ids_store.contains( from_city_name ) )
                city_ids_store.add_id( from_city_name );
            
            if ( !city_ids_store.contains( to_city_name ) )
                city_ids_store.add_id( to_city_name );

            if ( !transport_ids_store.contains( transport_name ) )
                transport_ids_store.add_id( transport_name );

            // инициализация полей
            (*this).from_city = city_ids_store.get( from_city_name );
            (*this).to_city = city_ids_store.get( to_city_name );
            (*this).transport_type = transport_ids_store.get( transport_name );
            (*this).time = stoi( cruise_time_str );
            (*this).fare = stoi( cruise_fare_str );
        }
        
        Edge( id &from_city, id to_city, id transport_type, cruiseTime cruise_time, cruiseFare cruise_fare ) {
            (*this).from_city = from_city;
            (*this).to_city = to_city;
            (*this).transport_type = transport_type;
            (*this).time = cruise_time;
            (*this).fare = cruise_fare;
        }
};

// вершина-сосед в графе (вершина, в которую ведет ребро Arc)
class AdjVer {
    public:
        std::tuple<id, id, cruiseTime, cruiseFare> vertex;

    public:
        AdjVer( const Edge &arc ) {
            id to_city = arc.to_city;
            id transport_type = arc.transport_type;
            cruiseTime cruise_time = arc.time;
            cruiseFare cruise_fare = arc.fare;

            vertex = std::make_tuple( to_city, transport_type, cruise_time, cruise_fare );
        }

        id get_to_city_id( ) {
            return std::get<0>( vertex );
        }

        id get_transport_id( ) {
            return std::get<1>( vertex );
        }

        cruiseTime get_cruise_time( ) {
            return std::get<2>( vertex );
        }

        cruiseFare get_cruise_fare( ) {
            return std::get<3>( vertex );
        }
};

class Graph {
    public:
        std::vector<std::vector<AdjVer>> adjList;
        int num_vers;
    public:
        Graph( const std::vector<Edge> &arcs, const int n ) {
            // n - число вершин
            adjList.resize( n );
            num_vers = n;

            for ( auto arc : arcs ) {
                adjList[ arc.from_city ].push_back( AdjVer( arc ) );
            }
        }

        std::vector<AdjVer> operator[] ( const id &from_city_id ) {
            return adjList.at( from_city_id );
        }
};

// пути на графе
class Path {
    public:
        std::vector<Edge> paths;
    public:
        Path( int num_vers ) {
            paths.resize( num_vers );
        }

        Path( std::vector<Edge> &arcs, int num_vers ) : paths(arcs) {
            paths.resize( num_vers );
        }

        Edge &operator[] ( int ind ) {
            return paths.at( ind );
        }

        // возвращает путь от from_city в to_city в path
        void get_path(const id &from_city, const id &to_city, std::vector<Edge> &path) {
            id temp_to_city = to_city;

            while ( temp_to_city != from_city ) {
                path.push_back( (*this)[temp_to_city] );
                temp_to_city = (*this)[temp_to_city].from_city;
            }
        }
};

// добавление нового элемента в путь
void operator+ ( Path &path, const Edge& arc ) {
            path[ arc.to_city ] = arc;
        }

#endif
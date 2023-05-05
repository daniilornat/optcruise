#include <vector>
#include <tuple>
#include <set>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string.h>

#ifndef GRAPH_H
#define GRAPH_H

//using namespace std;

typedef int city_id;
typedef int transport_id;
typedef int cruise_time;
typedef int cruise_fare;

// класс для хранения идентификаторов городов/транспорта и соответствующих им имен
class IDStore {
    public:
        std::vector<std::string> store;
    
    public:
        int size( ) {
            return store.size( );
        }

        int get_id( const std::string &name ) {
            std::vector<std::string>::iterator iter = find_if( store.begin( ), store.end( ), [name](const std::string& el)
                { return name == el; } );
            return std::distance( store.begin( ), iter );
        }

        std::string get_name ( const int &id ) {
            return store.at(id);
        }

        bool contains( const std::string &name ) {
            return find(store.begin(), store.end(), name) != store.end();
        }

    private:
        bool is_searched_elem( std::vector<std::string> &name1, std::vector<std::string> &name2 ) {
            return name1 == name2;
        }
};

// добавление нового идентификатора
void operator+ ( IDStore &store, const std::string &name ) {
    store.store.push_back( name );
}

// ребра графа
class Edge {
    public:
        city_id from_city, to_city;
        transport_id transport_type;
        cruise_time time;
        cruise_fare fare;

    public:
        Edge( )
            : from_city(0), to_city(0), transport_type(0), time(0), fare(0) {} 

        Edge( std::vector<std::string> &input_data, IDStore &city_ids_store, IDStore &transport_ids_store ) {
            std::string from_city_name = input_data[0],
                        to_city_name = input_data[1],
                        transport_name = input_data[2],
                        cruise_time_str = input_data[3],
                        cruise_fare_str = input_data[4];

            // добавление новых id если еще не добавлялись
            if ( !city_ids_store.contains( from_city_name ) )
                city_ids_store + from_city_name;
            
            if ( !city_ids_store.contains( to_city_name ) )
                city_ids_store + to_city_name;

            if ( !transport_ids_store.contains( transport_name ) )
                transport_ids_store + transport_name;

            // инициализация полей
            (*this).from_city = city_ids_store.get_id( from_city_name );
            (*this).to_city = city_ids_store.get_id( to_city_name );
            (*this).transport_type = transport_ids_store.get_id( transport_name );
            (*this).time = stoi( cruise_time_str );
            (*this).fare = stoi( cruise_fare_str );
        }

        Edge( city_id &from_city, city_id to_city, transport_id transport_type, cruise_time cruise_time, cruise_fare cruise_fare ) {
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
        std::tuple<city_id, transport_id, cruise_time, cruise_fare> vertex;

    public:
        AdjVer( const Edge &arc ) {
            city_id to_city = arc.to_city;
            transport_id transport_type = arc.transport_type;
            cruise_time cruise_time = arc.time;
            cruise_fare cruise_fare = arc.fare;

            vertex = std::make_tuple( to_city, transport_type, cruise_time, cruise_fare );
        }

        city_id get_to_city_id( ) {
            return std::get<0>( vertex );
        }

        transport_id get_transport_id( ) {
            return std::get<1>( vertex );
        }

        cruise_time get_cruise_time( ) {
            return std::get<2>( vertex );
        }

        cruise_fare get_cruise_fare( ) {
            return std::get<3>( vertex );
        }
};

// операторы сравнения для вершин-соседей
bool operator< ( AdjVer &left_val, AdjVer &right_val ) {
    return left_val.get_cruise_time( ) < right_val.get_cruise_time( );
}

bool operator> ( AdjVer &left_val, AdjVer &right_val ) {
    return left_val.get_cruise_time( ) > right_val.get_cruise_time( );
}

class Graph {
    public:
        std::vector<std::vector<AdjVer>> adjList;
        int num_vers;
    public:
        Graph( const std::vector<Edge> &arcs, int n ) {
            // n - число вершин
            num_vers = n;
            adjList.resize( n );

            for ( auto arc : arcs ) {
                adjList[ arc.from_city ].push_back( AdjVer( arc ) );
            }
        }

        std::vector<AdjVer> operator[] ( const city_id &from_city_id ) {
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
        void get_path(const city_id &from_city, city_id &to_city, std::vector<Edge> &path) {
            while ( to_city != from_city ) {
                path.push_back( (*this)[to_city] );
                to_city = (*this)[to_city].from_city;
            }
        }
};

// добавление нового элемента в путь
void operator+ ( Path &path, const Edge& arc ) {
            path[ arc.to_city ] = arc;
        }

#endif
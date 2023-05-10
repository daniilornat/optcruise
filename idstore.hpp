#ifndef IDSTORE
#define IDSTORE

#include <unordered_map>
#include <string>
#include<iostream>

typedef int id;
typedef int cruiseTime;
typedef int cruiseFare;

// класс для хранения идентификаторов городов/транспорта и соответствующих им имен
class IDStore {
    public:
        std::unordered_map<std::string, id> titles_to_ids;
        std::unordered_map<id, std::string> ids_to_titles;
    private:
        // значение последнего добавленного идентификатора
        id current_last_id = -1;
    public:
        int size( ) const {
            return titles_to_ids.size( );
        }

        // добавляет новый одентификатор
        void add_id( const std::string &title ) {
            current_last_id++;
            titles_to_ids.emplace( title, current_last_id );
            ids_to_titles.emplace( current_last_id, title );
        }

        // возвращает идентификатор по имени
        id get( const std::string &title ) const {
            const auto iter = titles_to_ids.find( title );
            if ( iter == titles_to_ids.end() ) {
                std::cout << "Ошибка: такого имени не существует" << std::endl;
                return -1;
            } else {
                return std::get<1>(*iter);
            }
        }

        // возвращает имя по идентификатору
        std::string get( const id &id ) const {
            const auto iter = ids_to_titles.find( id );
            if ( iter == ids_to_titles.end() ) {
                std::cout << "Ошибка: такого идентификатора не существует" << std::endl;
                return "";
            } else {
                return std::get<1>(*iter);
            }
        }

        // проверяет, содержится ли идентификатор id в idstore
        bool contains( const id &id ) const {
            return ids_to_titles.count( id );
        }

        // проверяет, содержится ли имя в idstore
        bool contains( const std::string &title ) const {
            return titles_to_ids.count( title );
        }
};

#endif

/*
class IDStore {
    public:
        std::vector<std::string> store;
    
    public:
        int size( ) const {
            return store.size( );
        }

        int get_id( const std::string &name ) const {
            std::vector<std::string>::const_iterator iter = find_if( store.begin( ), store.end( ), [name](const std::string& el)
                { return name == el; } );
            return std::distance( store.begin( ), iter );
        }

        std::string get_name ( const int &id ) const {
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
*/
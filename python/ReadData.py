from idStore import *
from Graph import *

def read_data( file_path ):
    graph = Graph( )
    city_ids_store = IDStore( )
    transport_ids_store = IDStore( )

    with open( file_path ) as input_data:
        for line in input_data:
            tokens = line.split( )
            tokens = [ token.replace("\"", "") for token in tokens ]

            from_city_title = tokens[0]
            to_city_title = tokens[1]
            transport_title = tokens[2]

            city_ids_store.add_id( from_city_title )
            city_ids_store.add_id( to_city_title )
            transport_ids_store.add_id( transport_title )

            from_city_id = city_ids_store.get_id( from_city_title )
            to_city_id = city_ids_store.get_id( to_city_title )
            transport_id = transport_ids_store.get_id( transport_title )

            graph.add_cruise( Edge( from_city_id, to_city_id, transport_id, int( tokens[3] ), int( tokens[4] ) ) )

    return graph, city_ids_store, transport_ids_store
import sqlite3


def connect ( name ):
    return sqlite3.connect( name )

def network ( connector ):
    name = connector.execute("SELECT Name FROM Network;").fetchone()                   # Name of the network
    dimension = connector.execute("SELECT Dimension FROM Network;").fetchone()         # Number of nodes in the network
    specification = connector.execute("SELECT Specification FROM Network;").fetchall() # Equations solved
    data=connector.execute("SELECT Graphviz FROM Network;").fetchall()                 # Graphviz network code
    graphviz_text=data[0][0]
    return [name,dimension,specification,graphviz_text]    

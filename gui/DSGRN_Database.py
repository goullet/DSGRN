import sqlite3


def connect ( name ):
    return sqlite3.connect( name )

def query ( connector, SQLcommand ):
    return connector.execute(SQLcommand).fetchall()

def network ( connector ):
    # name = connector.execute("SELECT Name FROM Network;").fetchone()                   # Name of the network
    name = query( connector, "SELECT Name FROM Network;" )
    dimension = connector.execute("SELECT Dimension FROM Network;").fetchone()         # Number of nodes in the network
    specification = connector.execute("SELECT Specification FROM Network;").fetchall() # Equations solved
    graphviz_text=connector.execute("SELECT Graphviz FROM Network;").fetchall()        # Graphviz network code
    return [name[0][0],dimension[0],specification[0][0],graphviz_text[0][0]]

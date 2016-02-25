import DSGRN_Database as dsgrnDB
from ipywidgets import widgets


def page_setup ( db ):
    [networkName,networkDimension,networkSpecification,networkGraphviz_text] = dsgrnDB.network(db)

    name = widgets.Text(description='Name: ', padding=4)
    name.padding = 4
    page = widgets.Box(children=[name])
    page.padding = 4

    return page

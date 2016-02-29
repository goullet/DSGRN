from ipywidgets import widgets
import DSGRN_Database as dsgrnDB
import DSGRN_Graphviz as dsgrnGV


def page_setup ( db ):
    [networkName,networkDimension,networkSpecification,networkGraphviz_text] = dsgrnDB.network(db)

    name = widgets.Text(description='Name: ', padding=4)
    name.padding = 4

    # gv = dsgrnGV . display_gv ( networkGraphviz_text )

    page = widgets.Box(children=[name])
    page.padding = 4

    return page

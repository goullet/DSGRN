from ipywidgets import widgets
from IPython.display import display

import DSGRN_Database as dsgrnDB


def page_setup ( db ):
    [networkName,networkDimension,networkSpecification,networkGraphviz_text] = dsgrnDB.network(db)

    name = widgets.Text(description='Query Command: ', padding=4)
    name.padding = 4


    def on_button_clicked(b):
        submitQuery()

    button = widgets.Button(description="Submit")
    button.on_click(on_button_clicked)
    display ( button )

    # "display(widgets.HBox(listButton))"

    page = widgets.Box(children=[name,button])
    page.padding = 4

    return page

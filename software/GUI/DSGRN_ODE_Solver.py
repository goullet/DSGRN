'''

'''
import DSGRN_Database as dsgrnDB
from ipywidgets import widgets
from IPython.display import display


def on_load_network_file_button_clicked(b):
    # pattern.append(str(buttonDict[str(b.description)].description+" "+ toggleDict[str(b.description)].value))
    # patternDisplay.value=mypattern()
    print "Not Implemented yet"


def page_setup ( ):

    networkFileText=''
    # Field text to enter the path of the network
    networkFileField = widgets.Textarea( description='Network File:',
                                         value=networkFileText,
                                         width=400,
                                         padding=4,
                                         height=35)

    networkFileLoadButton = widgets.Button ( description="Load network file" )
    networkFileLoadButton.on_click(on_load_network_file_button_clicked)



    display(widgets.HBox([networkFileField,networkFileLoadButton]))

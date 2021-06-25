TEMPLATE = subdirs
SUBDIRS += StandardItemModel \
           App \
           muparser \

 
 App.depends = StandardItemModel muparser


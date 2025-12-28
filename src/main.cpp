#include <QApplication>
#include "parser.h"
#include <QDebug>
#include "scene.hpp"
#include "view.hpp"
#include "items/node.hpp"

int main(int argc, char** argv) 
{
    QApplication app(argc, argv); 

    Parser parser;
    parser.parseFile("simpleExample.h");

    auto scene = QSchematic::Scene();
    auto view  = QSchematic::View();
    view.setScene(&scene);

    view.setWindowTitle("Code Graph");
    view.resize(800, 600);
    view.show();

    return app.exec();
}

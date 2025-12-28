#include "parser.h"
#include <tree_sitter/api.h>
#include "dataContainers.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

extern "C" const TSLanguage *tree_sitter_cpp();

void parse_tree(TSNode node,
                QSet<QString>& classes,
                QSet<Connection_dependency>& dependencies,
                QSet<Connection_inheritance>& inheritances,
                const char* source)
{
    const char* type = ts_node_type(node);

    // Если наш узел - класс, то обрабатываем
    if (strcmp(type, "class_specifier") == 0) 
    {
        // Получаем имя класса
        TSNode name_node = ts_node_child_by_field_name(node, "name", 4);
        QString class_name;
        if (!ts_node_is_null(name_node)) {
            uint32_t start = ts_node_start_byte(name_node);
            uint32_t end   = ts_node_end_byte(name_node);
            class_name = QString::fromUtf8(source + start, end - start);
            classes.insert(class_name);
        }

        for(uint32_t i = 0; i < ts_node_child_count(node); ++i) // Проходимся по всем детям
            {
            TSNode child = ts_node_child(node, i);

            if(strcmp(ts_node_type(child), "base_class_clause") == 0) // Читаем наследование
            {
                for(uint32_t j = 0; j < ts_node_child_count(child); ++j)
                {
                    TSNode grandChild = ts_node_child(child, j); // Потенциальный тип родительского класса
                    if(strcmp(ts_node_type(grandChild), "type_identifier") == 0)
                    {
                        uint32_t start = ts_node_start_byte(grandChild);
                        uint32_t end   = ts_node_end_byte(grandChild);
                        QString parentClass = QString::fromUtf8(source + start, end - start);
                        inheritances.insert({parentClass, class_name});
                    }
                }
            }
            else if(strcmp(ts_node_type(child), "field_declaration_list") == 0) // Читаем поля
            {
                TSNode& fieldDeclarationList = child;
                int n_fields = ts_node_named_child_count(fieldDeclarationList);
                for (int i = 0; i < n_fields; ++i) {
                    TSNode field_node = ts_node_named_child(child, i);
                    if (strcmp(ts_node_type(field_node), "field_declaration") == 0) {
                        // Тип поля
                        TSNode nodeType = ts_node_child_by_field_name(field_node, "type", 4);
                        bool isPrimitiveType = strcmp(ts_node_type(nodeType), "primitive_type") == 0;
                        if (!ts_node_is_null(nodeType) && !isPrimitiveType) {
                            uint32_t t_start = ts_node_start_byte(nodeType);
                            uint32_t t_end   = ts_node_end_byte(nodeType);
                            QString type_name = QString::fromUtf8(source + t_start, t_end - t_start);

                            dependencies.insert(Connection_dependency{class_name, type_name});
                        }
                    }
                }
            }

        }
    }

    // Рекурсивный обход всех named children
    int n_children = ts_node_child_count(node);
    for (int i = 0; i < n_children; ++i) {
        parse_tree(ts_node_child(node, i), classes, dependencies, inheritances, source);
    }
}

void print_tree(TSNode node, int depth = 0) 
{
    for (int i = 0; i < depth; i++)
        printf("  ");

    printf("%s\n", ts_node_type(node));
    uint32_t count = ts_node_child_count(node);
    for (uint32_t i = 0; i < count; i++) {
        print_tree(ts_node_child(node, i), depth + 1);
    }
}

bool Parser::parseFile(const QString& path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << QObject::tr("Couldn't open file ") << path;
    }
    QTextStream fs(&file);
    QString code = fs.readAll();

    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree* tree = ts_parser_parse_string(
        parser,
        nullptr,
        code.toUtf8(),
        code.size()
    );

    TSNode root = ts_tree_root_node(tree);

    mClasses.clear(); mDependencies.clear(); mInheritances.clear();
    parse_tree(root, mClasses, mDependencies, mInheritances, code.toUtf8());

    ts_tree_delete(tree);
    ts_parser_delete(parser);

    return true;
}
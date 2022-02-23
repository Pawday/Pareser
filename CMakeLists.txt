
project(Pareser)

add_library(Parser)

target_sources(Parser PRIVATE src/tokenizer.c)
target_sources(Parser PRIVATE src/number_parsers.c)
target_sources(Parser PRIVATE src/math_functions.c)

target_sources(Parser PRIVATE src/math_parser.c)
target_sources(Parser PRIVATE src/visual/token_dumper.c)
target_sources(Parser PRIVATE src/visual/tree_dumper.c)
target_sources(Parser PRIVATE src/visual/node_dumper.c)

target_include_directories(Parser PUBLIC include)

add_executable(ParserTester main.c)

target_link_libraries(ParserTester PRIVATE Parser)


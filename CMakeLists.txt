
project(Pareser)

add_library(Parser)

target_sources(Parser PRIVATE src/tokenizer.c)
target_sources(Parser PRIVATE src/number_parsers.c)
target_sources(Parser PRIVATE src/math_functions.c)
target_sources(Parser PRIVATE src/token_converter.c)
target_sources(Parser PRIVATE src/parser.c)

target_include_directories(Parser PUBLIC include)

add_executable(ParserTester main.c)

target_link_libraries(ParserTester PRIVATE Parser)


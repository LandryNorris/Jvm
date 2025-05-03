cd test_projects
./gradlew build
cd ..

mkdir cmake-build-testing
cd cmake-build-testing

cmake ..
cmake --build .

cd integration_tests/parser_tests
./parser_tests

cmake_minimum_required(VERSION 3.16)

project(FuzzyLogicLibrary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# Library
# ============================================================

add_library(
    fuzzy_logic

    # Membership functions
    src/Triangle.cpp
    src/Trapezoidal.cpp
    src/Gaussian.cpp
    src/Bell.cpp
    src/Sigmoid.cpp
    src/Singleton.cpp

    # Variables and sets
    src/FuzzySet.cpp
    src/FuzzyVariable.cpp
    src/InputVar.cpp
    src/OutputVar.cpp
    src/InputValue.cpp

    # Rules
    src/Antecedent.cpp
    src/Consequent.cpp
    src/Rule.cpp
    src/SugenoRule.cpp

    # Norms
    src/MinTNorm.cpp
    src/ProdTNorm.cpp
    src/MaxSNorm.cpp
    src/ProbSumSNorm.cpp

    # Inference engine
    src/Fuzzifier.cpp
    src/RuleEvaluator.cpp
    src/Aggregator.cpp
    src/InferenceEngine.cpp
    src/SugenoInference.cpp
    src/FuzzySystem.cpp

    # Defuzzifiers
    src/CentroidDefuzz.cpp
    src/BisectorDefuzz.cpp
    src/MOMDefuzz.cpp
    src/SOMDefuzz.cpp
    src/LOMDefuzz.cpp

    # Sugeno
    src/ConstSugenoConsequent.cpp
    src/LinearSugenoConsequent.cpp
    src/SugenoSystem.cpp
    src/MamdaniSystem.cpp

    # ========================================================
    # ML
    # ========================================================

    # Clustering
    src/ML/Clustering/FuzzyCMeans.cpp

    # Data
    src/ML/Data/Dataset.cpp
    src/ML/Data/CSVReader.cpp
    src/ML/Data/CSVWriter.cpp

    # Preprocessing
    src/ML/Preprocessing/StandardScaler.cpp
    src/ML/Preprocessing/MinMaxScaler.cpp

    # Generation
    src/ML/Generation/FuzzySetGenerator.cpp
)

target_include_directories(
    fuzzy_logic
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================
# GoogleTest
# ============================================================

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# ============================================================
# Tests
# ============================================================

add_executable(
    fuzzy_tests

    # Membership functions
    tests/test_triangle.cpp
    tests/test_trapezoidal.cpp
    tests/test_gaussian.cpp
    tests/test_bell.cpp
    tests/test_sigmoid.cpp
    tests/test_singleton.cpp

    # Variables
    tests/test_fuzzyset.cpp
    tests/test_fuzzyvariable.cpp
    tests/test_inputvalue.cpp

    # Rules
    tests/test_condition.cpp
    tests/test_antecedent.cpp
    tests/test_consequent.cpp
    tests/test_rule.cpp
    tests/test_sugenorule.cpp

    # Defuzzification
    tests/test_centroiddefuzz.cpp

    # Norms
    tests/test_tnorms.cpp
    tests/test_snorms.cpp

    # Engine
    tests/test_fuzzifier.cpp
    tests/test_aggregator.cpp
    tests/test_ruleevaluator.cpp
    tests/test_inferenceengine.cpp
    tests/test_sugenoinference.cpp
    tests/test_fuzzysystem.cpp
    tests/test_mamdanisystem.cpp
    tests/test_sugenosystem.cpp
    tests/test_integration.cpp

    # ========================================================
    # ML Tests
    # ========================================================

    tests/test_fuzzycmeans.cpp
    tests/test_dataset.cpp
    tests/test_csvreader.cpp
    tests/test_csvwriter.cpp
    tests/test_standardscaler.cpp
    tests/test_minmaxscaler.cpp
 tests/test_fuzzysetgenerator.cpp )

target_link_libraries(
    fuzzy_tests
    PRIVATE
    fuzzy_logic
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(fuzzy_tests)

# ============================================================
# Examples
# ============================================================

add_executable(
    fcm_example
    examples/fcm_example.cpp
)

target_link_libraries(
    fcm_example
    PRIVATE
    fuzzy_logic
)
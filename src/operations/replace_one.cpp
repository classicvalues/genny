#include "replace_one.hpp"
#include "parse_util.hpp"
#include <bsoncxx/json.hpp>
#include <stdlib.h>
#include <boost/log/trivial.hpp>

namespace mwg {

replace_one::replace_one(YAML::Node& node) {
    // need to set the name
    // these should be made into exceptions
    // should be a map, with type = find
    if (!node) {
        BOOST_LOG_TRIVIAL(fatal) << "Replace_One constructor and !node";
        exit(EXIT_FAILURE);
    }
    if (!node.IsMap()) {
        BOOST_LOG_TRIVIAL(fatal) << "Not map in replace_one type initializer";
        exit(EXIT_FAILURE);
    }
    if (node["type"].Scalar() != "replace_one") {
        BOOST_LOG_TRIVIAL(fatal)
            << "Replace_One constructor but yaml entry doesn't have type == replace_one";
        exit(EXIT_FAILURE);
    }
    if (node["options"])
        parseUpdateOptions(options, node["options"]);
    filter = makeDoc(node["filter"]);
    replacement = makeDoc(node["replacement"]);
    BOOST_LOG_TRIVIAL(debug) << "Added op of type replace_one";
}

// Execute the node
void replace_one::execute(mongocxx::client& conn, threadState& state) {
    auto collection = conn["testdb"]["testCollection"];
    bsoncxx::builder::stream::document mydoc{};
    bsoncxx::builder::stream::document myreplacement{};
    auto view = filter->view(mydoc, state);
    auto replacementview = replacement->view(mydoc, state);
    auto result = collection.replace_one(view, replacementview, options);
    BOOST_LOG_TRIVIAL(debug) << "replace_one.execute: replace_one is " << bsoncxx::to_json(view);
}
}

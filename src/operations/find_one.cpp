#include "find_one.hpp"
#include "parse_util.hpp"
#include <bsoncxx/json.hpp>
#include <stdlib.h>
#include <boost/log/trivial.hpp>

namespace mwg {

find_one::find_one(YAML::Node& node) {
    // need to set the name
    // these should be made into exceptions
    // should be a map, with type = find
    if (!node) {
        BOOST_LOG_TRIVIAL(fatal) << "Find_One constructor and !node";
        exit(EXIT_FAILURE);
    }
    if (!node.IsMap()) {
        BOOST_LOG_TRIVIAL(fatal) << "Not map in find_one type initializer";
        exit(EXIT_FAILURE);
    }
    if (node["type"].Scalar() != "find_one") {
        BOOST_LOG_TRIVIAL(fatal)
            << "Find_One constructor but yaml entry doesn't have type == find_one";
        exit(EXIT_FAILURE);
    }
    if (node["options"])
        parseFindOptions(options, node["options"]);
    filter = makeDoc(node["filter"]);
    BOOST_LOG_TRIVIAL(debug) << "Added op of type find_one";
}

// Execute the node
void find_one::execute(mongocxx::client& conn, threadState& state) {
    auto collection = conn["testdb"]["testCollection"];
    bsoncxx::builder::stream::document mydoc{};
    auto view = filter->view(mydoc, state);
    auto value = collection.find_one(view, options);
    BOOST_LOG_TRIVIAL(debug) << "find_one.execute: find_one is " << bsoncxx::to_json(view);
}
}

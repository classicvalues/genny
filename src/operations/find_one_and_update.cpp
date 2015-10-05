#include "find_one_and_update.hpp"
#include "parse_util.hpp"
#include <bsoncxx/json.hpp>
#include <stdlib.h>
#include <boost/log/trivial.hpp>

namespace mwg {

find_one_and_update::find_one_and_update(YAML::Node& node) {
    // need to set the name
    // these should be made into exceptions
    // should be a map, with type = find_one_and_update
    if (!node) {
        BOOST_LOG_TRIVIAL(fatal) << "Find_One_And_Update constructor and !node";
        exit(EXIT_FAILURE);
    }
    if (!node.IsMap()) {
        BOOST_LOG_TRIVIAL(fatal) << "Not map in find_one_and_update type initializer";
        exit(EXIT_FAILURE);
    }
    if (node["type"].Scalar() != "find_one_and_update") {
        BOOST_LOG_TRIVIAL(fatal) << "Find_One_And_Update constructor but yaml entry doesn't have "
                                    "type == find_one_and_update";
        exit(EXIT_FAILURE);
    }
    if (node["options"])
        parseFindOneAndUpdateOptions(options, node["options"]);
    filter = makeDoc(node["filter"]);
    update = makeDoc(node["update"]);
    BOOST_LOG_TRIVIAL(debug) << "Added op of type find_one_and_update";
}

// Execute the node
void find_one_and_update::execute(mongocxx::client& conn, threadState& state) {
    auto collection = conn["testdb"]["testCollection"];
    bsoncxx::builder::stream::document mydoc{};
    bsoncxx::builder::stream::document myupdate{};
    auto view = filter->view(mydoc, state);
    auto updateview = update->view(myupdate, state);
    auto value = collection.find_one_and_update(view, updateview, options);
    // need a way to exhaust the cursor
    BOOST_LOG_TRIVIAL(debug) << "find_one_and_update.execute: find_one_and_update is "
                             << bsoncxx::to_json(view);
}
}

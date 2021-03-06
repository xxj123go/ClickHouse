#pragma once

#include <Parsers/IAST.h>


namespace DB
{


/** OPTIMIZE query
  */
class ASTOptimizeQuery : public IAST
{
public:
    String database;
    String table;

    /// The partition to optimize can be specified.
    String partition;
    /// A flag can be specified - perform optimization "to the end" instead of one step.
    bool final;
    /// Do deduplicate (default: false)
    bool deduplicate;

    ASTOptimizeQuery() = default;
    ASTOptimizeQuery(const StringRange range_) : IAST(range_) {}

    /** Get the text that identifies this element. */
    String getID() const override { return "OptimizeQuery_" + database + "_" + table + "_" + partition + (final ? "_final" : "") + (deduplicate ? "_deduplicate" : ""); };

    ASTPtr clone() const override { return std::make_shared<ASTOptimizeQuery>(*this); }

protected:
    void formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override
    {
        settings.ostr << (settings.hilite ? hilite_keyword : "") << "OPTIMIZE TABLE " << (settings.hilite ? hilite_none : "")
            << (!database.empty() ? backQuoteIfNeed(database) + "." : "") << backQuoteIfNeed(table);

        if (!partition.empty())
            settings.ostr << (settings.hilite ? hilite_keyword : "") << " PARTITION " << (settings.hilite ? hilite_none : "")
                << partition;

        if (final)
            settings.ostr << (settings.hilite ? hilite_keyword : "") << " FINAL" << (settings.hilite ? hilite_none : "");

        if (deduplicate)
            settings.ostr << (settings.hilite ? hilite_keyword : "") << " DEDUPLICATE" << (settings.hilite ? hilite_none : "");
    }
};

}

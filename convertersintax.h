#ifndef CONVERTERSINTAX_H
#define CONVERTERSINTAX_H
#include <QString>
#include "node.h"
#include "shell.h"

///Define keywords for languange LsDyna .
namespace lsdynasintax {
/**
 * @brief The KeywordDyna enum Define keywords for languange LsDyna to move
 * different mode to interpret the data. All keywords are preceded by the symbol *.
 */
enum KeywordDyna
{
    $, /**< Identifies the part of the document it contains header. */
    KEYWORD, /**< Identifies the part of the document it contains other keyword not specified. */
    NODE, /**< Identifies the part of the document it contains list node declaration. */
    ELEMENTSHELL, /**< Identifies the part of the document it contains list element shell. */
    ELEMENTSOLID, /**< Identifies the part of the document it contains list element solid. */
    INITIALSTRAINSOLID, /**< Identifies the part of the document it contains initial strain on solid element. */
    INITIALSTRESSSHELL, /**< Identifies the part of the document it contains stress on shell element.*/
    END /**< Identifies the end of the document. */
};
}

/**
 * @brief The ConverterSintax class interpreter of the Ls Dyna format, reads the
 * sections of the document by checking that they correspond to the parts of
 * interest, leaves the specific operation when the header in the document
 * changes.
 */
class ConverterSintax
{
public:
    explicit ConverterSintax();

    ~ConverterSintax();

    void setInputLine(QString linefile, Node *node, Shell *shell);

    void test(QString p_linefile, Node *node, Shell *shell);

private:
    lsdynasintax::KeywordDyna _mode;
};

#endif // CONVERTERSINTAX_H

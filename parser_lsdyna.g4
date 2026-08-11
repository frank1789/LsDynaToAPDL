

// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

parser grammar parser_lsdyna;


options { tokenVocab=lexer_lsdyna; }

file_
    : statementList ? EOF
    ;

statementList
    : commandExpression
    | statementList commandExpression
    ;

commandExpression
    : Keyword
    | Node
    | ElementShellThickness
    | InitialStressShell
    | InitialStrainShell
    ;

coordinateExpression
    : Sign? FractionalConstant ExponentPart?
    | FractionalConstant
    | DigitSequence ExponentPart
    ;

nodeExpression
    : DigitSequence coordinateExpression coordinateExpression coordinateExpression coordinateExpression*
    ;

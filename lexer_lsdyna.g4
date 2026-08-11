

// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

lexer grammar lexer_lsdyna;

// *KEYWORD
Keyword
    : '*KEYWORD'
    ;

// *NODE
Node
    : '*NODE'
    ;

// *ELEMENT_SHELL_THICKNESS
ElementShellThickness
    : '*ELEMENT_SHELL_THICKNESS'
    ;

// *INITIAL_STRESS_SHELL
InitialStressShell
    : '*INITIAL_STRESS_SHELL'
    ;

// *INITIAL_STRAIN_SHELL
InitialStrainShell
    : '*INITIAL_STRAIN_SHELL'
    ;


fragment Nondigit
    : [a-zA-Z_]
    ;

fragment Digit
    : [0-9]
    ;

fragment FractionalConstant
    : DigitSequence? '.' DigitSequence
    | DigitSequence '.'
    ;

fragment ExponentPart
    : [eE] Sign? DigitSequence
    ;

fragment Sign
    : [+-]
    ;

DigitSequence
    : Digit+
    ;

// Common tokens
Whitespace
    : [ \t]+ -> channel(HIDDEN)
    ;

Newline
    : ('\r' '\n'? | '\n') -> channel(HIDDEN)
    ;

BlockComment
    : '$$' .*? -> channel(HIDDEN)
    ;

LineComment
    : '$$' ~[\r\n]* -> channel(HIDDEN)
    ;

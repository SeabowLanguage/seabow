/**
 * @file parser.hpp
 * @author LucaStarz
 * @brief Description of seabow's parser.
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_PARSER_HPP__
#define __SEABOW_PARSER_HPP__

#include "nodes/node_literal.hpp"

/**
 * @brief Seabow parser for seabow tokens convert tokens into nodes that represent statements of a seabow code execution.
*/
class Parser {
private:
    std::vector<Token*> tokens; // The seabow tokens to convert.
    sbw_ulong pos; // The actual position in the seabow tokens vector.
    std::unordered_map<sbw_string, NodeCompound*> *imports; // A dictionary that contains all

    /**
     * @brief Skip all following newline tokens.
    */
    sbw_none SkipNewLines(sbw_none);

    /**
     * @brief Advance the actual position in the seabow tokens vector.
     * @return The token at the old position.
    */
    Token *Advance(sbw_none);

    /**
     * @brief Get seabow token at the actual position.
     * @return The token at the actual position or the last token if the end of the vector is reached.
    */
    Token *Get(sbw_none); 
    
    /**
     * @brief Get seabow token at the actual position + an offset.
     * @param offset The offset to apply to the actual position.
     * @return The token at the specified position or the last token if the end of the vector is reached.
    */
    Token *Get(sbw_long offset);

    /**
     * @brief Compare the actual token type with an expected token type and return a node that represents an error if they are not equals.
     * @param t The type of token that is expected.
     * @param expected A text that specify what is exactly expected.
     * @return A seabow node that represents an error or nullptr if no error happened.
    */
    NodeBad *Match(sbw_token_type t, sbw_string expected);

    /**
     * @brief Read All tokens that composed a statement of the execution code.
     * @param is_stat A modifier to apply while reading tokens.
     * @return A node that represents a statement.
    */
    Node *ParseStatement(sbw_ubyte is_stat);

    /**
     * @brief Read all tokens that composed a group of multiple statements.
     * @param is_stat A modifier to apply while reading tokens.
     * @return A node that represents a group of multiple nodes (i.e. statements).
    */
    Node *ParseCompound(sbw_ubyte is_stat);

    /**
     * @brief Read all tokens that represents a seabow conversion type.
     * @return A node that represents a seabow conversion type.
    */
    Node *ParseConvertExpression(sbw_none);

    /**
     * @brief Read all tokens that composed a seabow declaration.
     * @param is_stat A modifier to apply while reading tokens. 
     * @return A node that represents a seabow declaration.
    */
    Node *ParseDeclaration(sbw_ubyte is_stat);

    /**
     * @brief Read a token that represents a seabow controller (i.e. break, continue, return).
     * @param ctrl A modifier that specify what kind of controller can be read.
     * @return A node that represents a seabow controller.
    */
    Node *ParseController(sbw_ubyte ctrl);
    
    /**
     * @brief Read all tokens that composed a seabow unary / binary calculation.
     * @param precedence The operator precedence of the old operator of the binary / unary calculation.
     * @return A node that represents a seabow unary / binary calculation.
    */
    Node *ParseBinaryUnaryExpression(sbw_ubyte precedence);

    /**
     * @brief Read all tokens that composed a seabow primary expression (i.e. number, character, string, ...).
     * @return A node that represents a seabow primary expression.
    */
    Node *ParsePrimaryExpression(sbw_none);

    /**
     * @brief Read all tokens that composed a seabow question operation (i.e. ?(cond) : expr1 : expr2).
     * @return A node that represents a seabow question operation.
    */
    Node *ParseQuestionOperator(sbw_none);
public:
    /**
     * @brief Construct a new seabow parser to analyze seabow tokens.
     * @param tokens The seabow tokens to analyze.
     * @param imports The imported files of the parent parser.
    */
    Parser(std::vector<Token*> tokens, std::unordered_map<sbw_string, NodeCompound*> *imports);

    /**
     * @brief Destroy a seabow parser.
    */
    inline virtual ~Parser() {
        if (this->imports) { delete this->imports; }
    }

    /**
     * @brief Read all the seabow tokens and translate it into a node that contains all the other generated nodes.
     * @return The 'root' node.
    */
    NodeCompound *Parse(sbw_none);

    /**
     * @brief 
     * @param tokens 
     * @return sbw_none 
    */
    sbw_none ReUse(std::vector<Token*> tokens);

    /**
     * @brief Get the dictionary of all imported seabow files.
     * @return The pointer to the dictionary of all imported seabow files.
    */
    inline std::unordered_map<sbw_string, NodeCompound*> *Imports(sbw_none) const { return this->imports; }
};

#endif // __SEABOW_PARSER_HPP__
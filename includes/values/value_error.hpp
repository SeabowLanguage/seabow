/**
 * @file value_error.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a seabow error.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_ERROR_HPP__
#define __SEABOW_VALUE_ERROR_HPP__

#include "value.hpp"

/**
 * @brief A seabow value that represents a seabow error.
*/
class SBW_ValueError: public SBW_Value {
private:
    sbw_string name, details; // Name and details of the seabow error. 
    sbw_ulong line, column; // The position of the seabow error.

public:
    /**
     * @brief Construct a new seabow valueerror with specified position.
     * @param n The name of the seabow error.
     * @param d The details of the seabow error.
     * @param l The line position of the seabow error.
     * @param c The column position of the seabow error.
    */
    SBW_ValueError(sbw_string n, sbw_string d, sbw_ulong l, sbw_ulong c);
    
    /**
     * @brief Construct a new seabow valueerror without position information.
     * @param n The name of the seabow error.
     * @param d The details of the seabow error.
    */
    SBW_ValueError(sbw_string n, sbw_string d);

    /**
     * @brief Destroy a seabow valueerror.
    */
    inline virtual ~SBW_ValueError(sbw_none) {}

    /**
     * @brief Get the seabow value's type of a seabow valueerror (i.e. SBW_ERROR).
     * @return SBW_ERROR.
    */
    inline sbw_value_type Type(sbw_none) const { return SBW_ERROR; }
    
    /**
     * @brief Get the name of the seabow valueerror.
     * @return The name of the seabow valueerror.
    */
    inline sbw_string Name(sbw_none) { return this->name; }
    
    /**
     * @brief Get the details of the seabow valueerror.
     * @return The details of the seabow valueerror.
    */
    inline sbw_string Details(sbw_none) { return this->details; }
    
    /**
     * @brief Get the line position of the seabow valueerror.
     * @return The line position of the seabow valueerror.
    */
    inline sbw_ulong Line(sbw_none) { return this->line; }

    /**
     * @brief Get the column position of the seabow valueerror.
     * @return The column position of the seabow valueerror.
    */
    inline sbw_ulong Column(sbw_none) { return this->column; }

    /**
     * @brief Set the position information of the seabow valueerror.
     * @param l The line position to set.
     * @param c The column position to set.
    */
    inline sbw_none SetPos(sbw_ulong l, sbw_ulong c) { this->line = l; this->column = c; }

    /**
     * @brief Check if the seabow error is null.
     * @return true if the error is null, otherwise false.
    */
    inline sbw_bool IsNull(sbw_none) { return this->name.empty(); }
};

#endif // __SEABOW_VALUE_ERROR_HPP__
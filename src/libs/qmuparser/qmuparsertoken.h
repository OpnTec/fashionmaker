/***************************************************************************************************
 **
 **  Original work Copyright (C) 2013 Ingo Berg
 **  Modified work Copyright 2014 Roman Telezhynskyi <dismine(at)gmail.com>
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#ifndef QMUPARSERTOKEN_H
#define QMUPARSERTOKEN_H

#include <cassert>
#include <string>
#include <stack>
#include <memory>
#include <QtGlobal>

#include "qmuparsererror.h"
#include "qmuparsercallback.h"

/** @file
    @brief This file contains the parser token definition.
*/

namespace qmu
{
/**
 * @brief Encapsulation of the data for a single formula token.
 *
 * Formula token implementation. Part of the Math Parser Package.
 * Formula tokens can be either one of the following:
 * <ul>
 *  <li>value</li>
 *  <li>variable</li>
 *  <li>function with numerical arguments</li>
 *  <li>functions with a string as argument</li>
 *  <li>prefix operators</li>
 *  <li>infix operators</li>
 *  <li>binary operator</li>
 * </ul>
 *
 * @author (C) 2004-2013 Ingo Berg
 */
template<typename TBase, typename TString>
class QmuParserToken
{
public:
    //---------------------------------------------------------------------------
    /**
    * @brief Constructor (default).
    *
    * Sets token to an neutral state of type cmUNKNOWN.
    * @throw nothrow
    * @sa ECmdCode
    */
    QmuParserToken()
        : m_iCode ( cmUNKNOWN ), m_iType ( tpVOID ), m_pTok ( 0 ), m_iIdx ( -1 ), m_strTok(), m_strVal(), m_fVal(),
          m_pCallback()
    {}

    //------------------------------------------------------------------------------
    /**
     * @brief Create token from another one.
     *
     * Implemented by calling Assign(...)
     * @throw nothrow
     * @post m_iType==cmUNKNOWN
     * @sa #Assign
     */
    QmuParserToken ( const QmuParserToken &a_Tok )
        : m_iCode ( a_Tok.m_iCode ), m_iType ( a_Tok.m_iType ), m_pTok ( a_Tok.m_pTok ), m_iIdx ( a_Tok.m_iIdx ),
          m_strTok( a_Tok.m_strTok ), m_strVal(a_Tok.m_strVal), m_fVal(a_Tok.m_fVal), m_pCallback()
    {
        Assign ( a_Tok );
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Assignement operator.
     *
     * Copy token state from another token and return this.
     * Implemented by calling Assign(...).
     * @throw nothrow
     */
    QmuParserToken& operator= ( const QmuParserToken &a_Tok )
    {
        if ( &a_Tok == this )
        {
            return *this;
        }
        Assign ( a_Tok );
        return *this;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Copy token information from argument.
     *
     * @throw nothrow
     */
    void Assign ( const QmuParserToken &a_Tok )
    {
        m_iCode = a_Tok.m_iCode;
        m_pTok = a_Tok.m_pTok;
        m_strTok = a_Tok.m_strTok;
        m_iIdx = a_Tok.m_iIdx;
        m_strVal = a_Tok.m_strVal;
        m_iType = a_Tok.m_iType;
        m_fVal = a_Tok.m_fVal;
        // create new callback object if a_Tok has one
        m_pCallback.reset ( a_Tok.m_pCallback.get() ? a_Tok.m_pCallback->Clone() : 0 );
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Assign a token type.
     *
     * Token may not be of type value, variable or function. Those have seperate set functions.
     *
     * @pre [assert] a_iType!=cmVAR
     * @pre [assert] a_iType!=cmVAL
     * @pre [assert] a_iType!=cmFUNC
     * @post m_fVal = 0
     * @post m_pTok = 0
     */
    QmuParserToken& Set ( ECmdCode a_iType, const TString &a_strTok = TString() )
    {
        // The following types cant be set this way, they have special Set functions
        assert ( a_iType != cmVAR );
        assert ( a_iType != cmVAL );
        assert ( a_iType != cmFUNC );

        m_iCode = a_iType;
        m_iType = tpVOID;
        m_pTok = 0;
        m_strTok = a_strTok;
        m_iIdx = -1;

        return *this;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Set Callback type.
     */
    QmuParserToken& Set ( const QmuParserCallback &a_pCallback, const TString &a_sTok )
    {
        assert ( a_pCallback.GetAddr() );

        m_iCode = a_pCallback.GetCode();
        m_iType = tpVOID;
        m_strTok = a_sTok;
        m_pCallback.reset ( new QmuParserCallback ( a_pCallback ) );

        m_pTok = 0;
        m_iIdx = -1;

        return *this;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Make this token a value token.
     *
     * Member variables not necessary for value tokens will be invalidated.
     * @throw nothrow
     */
    QmuParserToken& SetVal ( TBase a_fVal, const TString &a_strTok = TString() )
    {
        m_iCode = cmVAL;
        m_iType = tpDBL;
        m_fVal = a_fVal;
        m_strTok = a_strTok;
        m_iIdx = -1;

        m_pTok = 0;
        m_pCallback.reset ( 0 );

        return *this;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief make this token a variable token.
     *
     * Member variables not necessary for variable tokens will be invalidated.
     * @throw nothrow
     */
    QmuParserToken& SetVar ( TBase *a_pVar, const TString &a_strTok )
    {
        m_iCode = cmVAR;
        m_iType = tpDBL;
        m_strTok = a_strTok;
        m_iIdx = -1;
        m_pTok = reinterpret_cast<void*> ( a_pVar );
        m_pCallback.reset ( 0 );
        return *this;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Make this token a variable token.
     *
     * Member variables not necessary for variable tokens will be invalidated.
     * @throw nothrow
     */
    QmuParserToken& SetString ( const TString &a_strTok, std::size_t a_iSize )
    {
        m_iCode = cmSTRING;
        m_iType = tpSTR;
        m_strTok = a_strTok;
        m_iIdx = static_cast<int> ( a_iSize );

        m_pTok = 0;
        m_pCallback.reset ( 0 );
        return *this;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Set an index associated with the token related data.
     *
     * In cmSTRFUNC - This is the index to a string table in the main parser.
     * @param a_iIdx The index the string function result will take in the bytecode parser.
     * @throw QmuParserError if #a_iIdx<0 or #m_iType!=cmSTRING
     */
    void SetIdx ( int a_iIdx )
    {
        if ( m_iCode != cmSTRING || a_iIdx < 0 )
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        m_iIdx = a_iIdx;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Return Index associated with the token related data.
     *
     * In cmSTRFUNC - This is the index to a string table in the main parser.
     *
     * @throw QmuParserError if #m_iIdx<0 or #m_iType!=cmSTRING
     * @return The index the result will take in the Bytecode calculatin array (#m_iIdx).
     */
    int GetIdx() const
    {
        if ( m_iIdx < 0 || m_iCode != cmSTRING )
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        return m_iIdx;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Return the token type.
     *
     * @return #m_iType
     * @throw nothrow
     */
    ECmdCode GetCode() const
    {
        if ( m_pCallback.get() )
        {
            return m_pCallback->GetCode();
        }
        else
        {
            return m_iCode;
        }
    }

    //------------------------------------------------------------------------------
    ETypeCode GetType() const
    {
        if ( m_pCallback.get() )
        {
            return m_pCallback->GetType();
        }
        else
        {
            return m_iType;
        }
    }

    //------------------------------------------------------------------------------
    int GetPri() const
    {
        if ( m_pCallback.get() == nullptr)
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        if ( m_pCallback->GetCode() != cmOPRT_BIN && m_pCallback->GetCode() != cmOPRT_INFIX )
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        return m_pCallback->GetPri();
    }

    //------------------------------------------------------------------------------
    EOprtAssociativity GetAssociativity() const
    {
        if ( m_pCallback.get() == nullptr || m_pCallback->GetCode() != cmOPRT_BIN )
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        return m_pCallback->GetAssociativity();
    }

    //------------------------------------------------------------------------------
    template < class FunctionPtr >
    FunctionPtr union_cast( void* objectPtr ) const
    {
        union
        {
            void* obj;
            FunctionPtr func;
        } var;

        Q_STATIC_ASSERT_X(sizeof(void *) == sizeof(void (*)(void)),
                          "object pointer and function pointer sizes must equal");

        var.obj = objectPtr;
        return var.func;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Return the address of the callback function assoziated with function and operator tokens.
     *
     * @return The pointer stored in #m_pTok.
     * @throw QmuParserError if token type is non of:
     *        <ul>
     *           <li>cmFUNC</li>
     *           <li>cmSTRFUNC</li>
     *           <li>cmPOSTOP</li>
     *           <li>cmINFIXOP</li>
     *           <li>cmOPRT_BIN</li>
     *         </ul>
     * @sa ECmdCode
     */
    generic_fun_type GetFuncAddr() const
    {
        return ( union_cast<generic_fun_type>( m_pCallback.get() ) ) ?
                    union_cast<generic_fun_type>( m_pCallback->GetAddr() ) : union_cast<generic_fun_type>(0);
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Get value of the token.
     *
     * Only applicable to variable and value tokens.
     * @throw QmuParserError if token is no value/variable token.
     */
    TBase GetVal() const
    {
        switch ( m_iCode )
        {
            case cmVAL:
                return m_fVal;
            case cmVAR:
                return * ( reinterpret_cast<TBase*>(m_pTok) );
            case cmLE:
            case cmGE:
            case cmNEQ:
            case cmEQ:
            case cmLT:
            case cmGT:
            case cmADD:
            case cmSUB:
            case cmMUL:
            case cmDIV:
            case cmPOW:
            case cmLAND:
            case cmLOR:
            case cmASSIGN:
            case cmBO:
            case cmBC:
            case cmIF:
            case cmELSE:
            case cmENDIF:
            case cmARG_SEP:
            case cmVARPOW2:
            case cmVARPOW3:
            case cmVARPOW4:
            case cmVARMUL:
            case cmPOW2:
            case cmFUNC:
            case cmFUNC_STR:
            case cmFUNC_BULK:
            case cmOPRT_BIN:
            case cmOPRT_POSTFIX:
            case cmOPRT_INFIX:
            case cmEND:
            case cmUNKNOWN:
            case cmSTRING:
            default:
                throw QmuParserError ( ecVAL_EXPECTED );
        }
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Get address of a variable token.
     *
     * Valid only if m_iType==CmdVar.
     * @throw QmuParserError if token is no variable token.
     */
    TBase* GetVar() const
    {
        if ( m_iCode != cmVAR )
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        return reinterpret_cast<TBase*>( m_pTok );
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Return the number of function arguments.
     *
     * Valid only if m_iType==CmdFUNC.
     */
    int GetArgCount() const
    {
        assert ( m_pCallback.get() );

        if ( m_pCallback->GetAddr() == nullptr)
        {
            throw QmuParserError ( ecINTERNAL_ERROR );
        }

        return m_pCallback->GetArgc();
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Return the token identifier.
     *
     * If #m_iType is cmSTRING the token identifier is the value of the string argument
     * for a string function.
     * @return #m_strTok
     * @throw nothrow
     * @sa m_strTok
     */
    const TString& GetAsString() const
    {
        return m_strTok;
    }
private:
    ECmdCode  m_iCode;  ///< Type of the token; The token type is a constant of type #ECmdCode.
    ETypeCode m_iType;
    void  *m_pTok;      ///< Stores Token pointer; not applicable for all tokens
    int  m_iIdx;        ///< An otional index to an external buffer storing the token data
    TString m_strTok;   ///< Token string
    TString m_strVal;   ///< Value for string variables
    qreal m_fVal;  ///< the value
    std::unique_ptr<QmuParserCallback> m_pCallback;
};
} // namespace qmu

#endif

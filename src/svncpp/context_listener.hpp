/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_CONTEXT_LISTENER_HPP_
#define _SVNCPP_CONTEXT_LISTENER_HPP_ 

// stl
#include <string>

// Subversion api
#include "svn_client.h"

// svncpp
#include "pool.hpp"

// forward declarations

namespace svn
{
  /**
   * This is the interface that is used by @a Context
   * for callbacks.
   * To use this you will have to inherit from this
   * interface and overwrite the virtual methods.
   */
  class ContextListener
  {
  public:
    /**
     * this method will be called to retrieve
     * authentication information
     *
     * WORKAROUND FOR apr_xlate PROBLEM: 
     * STRINGS ALREADY HAVE TO BE UTF8!!!
     *
     * @param username
     * @param realm in which username/password will be used
     * @param password
     * @return continue action?
     * @retval true continue
     */
    virtual bool 
    contextGetLogin (const std::string & realm,
                     std::string & username, 
                     std::string & password) = 0;

    /** 
     * this method will be called to notify about
     * the progress of an ongoing action
     *
     * @param path
     * @param action
     * @param kind
     * @param mime_type
     * @param content_state
     * @param prop_state
     * @param revision
     */
    virtual void
    contextNotify (const char *path,
                   svn_wc_notify_action_t action,
                   svn_node_kind_t kind,
                   const char *mime_type,
                   svn_wc_notify_state_t content_state,
                   svn_wc_notify_state_t prop_state,
                   svn_revnum_t revision) = 0;

    /**
     * this method will be called to retrieve
     * a log message
     *
     * WORKAROUND FOR apr_xlate PROBLEM: 
     * STRINGS ALREADY HAVE TO BE UTF8!!!
     *
     * @param msg log message
     * @return continue action?
     * @retval true continue
     */
    virtual bool
    contextGetLogMessage (std::string & msg) = 0;

    /**
     * @see contextSslServerPrompt
     */
    struct SslServerPromptData
    {
    public:
      /** the following data is requested */
      bool trustPermanently;
      int acceptedFailures;

      /** failure count */
      const int failures;

      /** certificate information */
      std::string hostname;
      std::string fingerprint;
      std::string validFrom;
      std::string validUntil;
      std::string issuerDName;

      SslServerPromptData (const int failures = 0);
    };

    /**
     * this method is called if there is a certificate,
     * that has to be confirmed by the user
     *
     * @param data 
     * @retval false prompt was cancelled
     */
    virtual bool
    contextSslServerPrompt (SslServerPromptData & data) = 0;

    /**
     * this method is called to retrieve client side
     * information
     */
    virtual bool 
    contextSslClientPrompt (std::string & certFile) = 0;

    /**
     * this method is called to retrieve the password
     * for the certificate
     *
     * @param password
     */
    virtual bool
    contextSslPwPrompt (std::string & password) = 0;
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */

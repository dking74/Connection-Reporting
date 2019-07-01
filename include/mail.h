#ifndef _MAIL_H
#define _MAIL_H

#include <quickmail.h>

#include <vector>

namespace mail {

/**
 * Function to initialize a mail object with mail criteria
 * 
 * @param: const char* to_address[]
 *       : The address(es) to send to
 * @param: const char* cc_address[]
 *       : The address(es) to include on message
 * @param: const char* from_addres
 *       : The address to send from
 * @param: const char* subject
 *       : The subject of the message
 * @param: const char* message
 *       : The message to send
 * @param: const char* attachments[]
 *       : The attachments to send with the message
 * 
 * @rtype: quickmail mailobj
 *       : The mail object that was constructed
 */
quickmail initialize_mail(std::vector<const char*> to_address,
                          std::vector<const char*> cc_address,
                          const char* from_address,
                          const char* subject,
                          const char* message, 
                          std::vector<const char*> attachments);

/**
 * Function to actually send a mail message
 *
 * @param: const quickmail mail_obj
 *       : A mail object constructed with 'initialize_mail'
 * @param: const char* smtp_server
 *       : The smtp server to send email through
 * @param: const int smtp_point; default = 25
 *       : The port to connect to server with
 * @param: const char* smtp_user; default = NULL
 *       : The username to connect to smtp with
 * @param: const char* smtp_pass; default = NULL
 *       : The password with the associated username
 * 
 * @rtype: void
 */
void send_mail(const quickmail mail_obj,
               const char* smtp_server, 
               const int smtp_port = 25, 
               const char* smtp_user = NULL, 
               const char* smtp_pass = NULL);

}; // namespace mail

#endif

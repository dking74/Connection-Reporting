#include "mail.h"

#include <cstdio>
#include <cstring>

namespace mail {

quickmail initialize_mail(std::vector<const char*> to_address,
                          std::vector<const char*> cc_address,
                          const char* from_address,
                          const char* subject,
                          const char* message, 
                          std::vector<const char*> attachments) {
  quickmail_initialize();
  quickmail mailobj = quickmail_create(from_address, subject);
  
  // Add all to addresses to quickmail object
  for (auto& to : to_address)
    quickmail_add_to(mailobj, to);

  // Add all cc addresses to quickmail object
  for (auto& cc : cc_address)
    quickmail_add_cc(mailobj, cc);

  // Place mail headers on object to ensure proper delivery
  quickmail_add_header(mailobj, "Importance: Medium");
  quickmail_add_header(mailobj, "X-Priority: 5");
  quickmail_add_header(mailobj, "X-MSMail-Priority: Low");
  quickmail_add_body_memory(mailobj, "text/html", (char*)message, strlen(message), 0);

  for (auto& attachment : attachments)
    quickmail_add_attachment_file(mailobj, attachment, NULL); 

  return mailobj;
}

void send_mail(const quickmail mail_obj,
               const char* smtp_server, 
               const int smtp_port, 
               const char* smtp_user, 
               const char* smtp_pass) {
  const char* errmsg;
  quickmail_set_debug_log(mail_obj, stderr);
  if ((errmsg = quickmail_send(mail_obj, smtp_server, smtp_port, smtp_user, smtp_pass)) != NULL)
    fprintf(stderr, "Error sending e-mail: %s\n", errmsg);
  quickmail_destroy(mail_obj);
  quickmail_cleanup();
}

}; // namespace mail

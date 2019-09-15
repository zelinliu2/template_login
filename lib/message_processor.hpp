/*
* Filename: message_processor.hpp
* Author: Zelin Liu
* Date: 2019/09/12
* Description:
* Instruction:
*/
#ifndef _MESSAGE_PROCESSOR_HPP_
#define _MESSAGE_PROCESSOR_HPP_
class Message_processor
{
public:
  Message_processor(std::shared_ptr<DB_core> db, std::shared_ptr<std::deque<std::shared_ptr<Message>>> queue);
  ~Message_processor();
  void process_messages(std:;vector<std::shared_ptr<Message_body>> input);
  static unsigned short binary_2_short(char * data);
  static void get_username_and_password(char* data, std::string &username, std::string &password);
private:
  void process_one_msg(std::shared_ptr<Message_body> msg);

  std::shared_ptr<std::deque<std::shared_ptr<Message>>> send_queue;
  DB_core db;
  void do_sign_up(std::shared_ptr<Message_body> msg);
  void do_sign_in(std::shared_ptr<Message_body> msg);


};
#endif

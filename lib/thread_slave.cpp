/*
* Filename: thread_slave.hpp
* Author: Zelin Liu
* Date: 2019/08/14
* Description: Thread_slave implementations
* Instruction:
*/

#include "thread_slave.hpp"
#include "channel_master_slave.hpp"

/*
*
*/
Thread_slave::Thread_slave(std::shared_ptr<Channel_master_slave> comm, int id)
{
  this->channel = comm;
  this->id = id;
  this->num_session = 0;
  this->pollfd_list = (struct pollfd*)calloc(SLAVE_SOCKET_PER_THREAD_MAX, sizeof(struct pollfd));
}

/*
*
*/
  ~Thread_slave()
  {
    free(pollfd_list);
  }

/*
*
*/
void Thread_slave::thread_function()
{
  while(true)
  {
    std::vector<int> new_socks;

    // try getting socket from queue until queue is locked or queue is empty
    // current socks and received sockets should not exceed max allowed sockets

    int num_new_socks_received = 0;
    while(this->num_session + num_new_socks_received < SLAVE_SOCKET_PER_THREAD_MAX)
    {
      int temp_sock = -1;
      if(this->channel.pop(&temp_sock, 0))
      {
        new_socks.push_back(temp_sock);
        num_new_socks_received ++;
      }
      else
      {
        break;
      }
    }



    for(int i = 0; i < num_new_socks_received; i++)
    {
      // make a new session on heap
      output.push_back(
      std::shared_ptr<Session> session_temp = std::make_shared<Session>(new_socks[i]);

      // if there is no gaps
      // then append new session and sockfd to the end
      if(this->gaps.empty())
      {
        this->session_list.push_back(session_temp);
        this->init_pollfd(this->pollfd_list[num_session], new_socks[i]);
      }
      else
      // if there are gaps
      // fill the gaps
      {
          this->session_list[this->gaps.back()] = session_temp;
          this->init_pollfd(this->pollfd_list[this->gaps.back()], new_socks[i]);
          this->gaps.pop_back();
      }
      num_session++;
    }

    this->rearrage();

    std::vector<int> polled_sessions = this->mypoll()

    int num_active = polled_sessions.size();

    if(num_active == 0)
    {
      std::this_thread::yield();
    }
    else
    {
      for(int j = 0; j < num_active;  j++)
      {
        session_list[j].do_session();
      }


      // not sure if here
    }

    // or here
    // drop connection
      // close fd
      // decrement num_session
      // add to gaps





  }
}

/*
*
*/
void Thread_slave::swap_pollfd_and_session_list(int important, int throwaway)
{
  this->pollfd_list[throwaway] = this->pollfd_list[important];
  this->session_list[throwaway] = this->session_list[important];
}

/*
*
*/
void init_pollfd(struct pollfd &fd, int sock)
{
  fd.fd = sock;
  fd.events = POLLIN;
}

/*
*
*/
void Thread_slave::rearrange()
{
  // if no gaps, then no need to rearrange
  if(this->gaps.empty())
  {
    return;
  }

  // first, find out all the gaps between 0<= gaps <num_session, these are gaps that need to be filled
  // then find out all the valid between num_session<= valid < num_gaps + num_sessions

  std::vector<int> to_be_fill;
  std::vector<int> to_be_swap_in;
  for(int j = this->num_session; j < this->gaps.size() + this->num_session; j++)
  {
    to_be_swap_in.push_back(j);
  }

  for(int i = 0; i < this->gaps.size(); i++)
  {
    if(this->gaps[i] < num_session)
    {
      to_be_filled.push_back(this->gaps[i]);
    }
    else
    {
      std::swap(to_be_swap_in[this->gaps[i]-this->num_session], to_be_swap_in.back());
      to_be_swap_in.pop_back();
    }
  }

  for(int k = 0; k < to_be_fill.size(); k++)
  {
    this->swap_pollfd_and_session_list(to_be_swap_in[k], to_be_fill[k]);
  }
  this->gaps.clear();
}

/*
*
*/
std::vector<int> Thread_slave::mypoll()
{
  std::vector<int> output;
  int num_active = poll(this->pollfd_list, num_session, SLAVE_POLL_TIMEOUT);
  for(int i = 0; i < num_session; i ++){
    if(this->pollfd_list[i].revents && POLLIN)
    {
      output.push_back(i);
    }
  }
  if(output.size() != num_active)
  {
    throw std::runtime_error("poll() revents and return miss-match");
  }
  return output;
}

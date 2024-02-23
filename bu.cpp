#include "Server.hpp"

int main() {
    int server_fd, rc, max_sd, new_sd, on = 1, len;
	// int server_fd, new_socket, rc, max_sd, new_sd, len;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
	// int    desc_ready, end_server = false;
	int    end_server = false;
	fd_set	readfds, writefds, tmp_readfds, tmp_writefds;
	struct timeval      timeout;
	int    close_conn;
	char   buffer[3000];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
        perror("socket failed");
        return(1);
    }

	rc = setsockopt(server_fd, SOL_SOCKET,  SO_REUSEADDR,
					(char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(server_fd);
		return(1);
	}
    rc = fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    if (rc < 0)
        return (1);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
        perror("bind failed");
        return(1);
    }

    // Listening
    if (listen(server_fd, 32) == -1)
	{
        perror("listen");
        return(1);
    }

// fd_set	readfds, writefds, tmp_readfds, tmp_writefds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&tmp_readfds);
	FD_ZERO(&tmp_writefds);
	FD_SET(server_fd, &readfds);

	max_sd = server_fd;
   	// FD_SET(server_fd, &master_set);
	timeout.tv_sec  = 3 * 60;
	timeout.tv_usec = 0;

    while (end_server == false)
	{
		std::memcpy(&tmp_readfds, &readfds, sizeof(readfds));
		std::memcpy(&tmp_writefds, &writefds, sizeof(writefds));

		rc = select(max_sd + 1, &tmp_readfds, &tmp_writefds, NULL, &timeout);

		if (rc < 0)
		{
			perror("  select() failed");
			break;
		}

		/**********************************************************/
		/* Check to see if the 3 minute time out expired.         */
		/**********************************************************/
		if (rc == 0)
		{
			printf("  select() timed out.  End program.\n");
			break;
		}
		std::cout << "Waiting for connection..." << std::endl;

		// desc_ready = rc;
		for (int i = 0; i <= max_sd; i++)
		{
			/*******************************************************/
			/* Check to see if this descriptor is ready            */
			/*******************************************************/
			// if (FD_ISSET(i, &tmp_readfds))
			// {
				/****************************************************/
				/* A descriptor was found that was readable - one   */
				/* less has to be looked for.  This is being done   */
				/* so that we can stop looking at the working set   */
				/* once we have found all of the descriptors that   */
				/* were ready.                                      */
				/****************************************************/
				// desc_ready -= 1;

				/****************************************************/
				/* Check to see if this is the listening socket     */
				/****************************************************/
				if (i == server_fd && FD_ISSET(i, &tmp_readfds))
				{
					printf("  Listening socket is readable\n");
					/*************************************************/
					/* Accept all incoming connections that are      */
					/* queued up on the listening socket before we   */
					/* loop back and call select again.              */
					/*************************************************/
					// while (new_sd != -1)
					// {
						/**********************************************/
						/* Accept each incoming connection.  If       */
						/* accept fails with EWOULDBLOCK, then we     */
						/* have accepted all of them.  Any other      */
						/* failure on accept will cause us to end the */
						/* server.                                    */
						/**********************************************/
						new_sd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
						if (new_sd < 0)
						{
							if (errno != EWOULDBLOCK)
							{
							perror("  accept() failed");
							end_server = true;
							}
							break;
						}

						/*************				std::ifstream file("./index.html");
						std::stringstream htmlResponse;
						htmlResponse << file.rdbuf();
						file.close();
						std::string content = htmlResponse.str();
						rc = send(i, content.c_str(), content.size(), 0);
						// rc = send(i, buffer, len, 0);
						if (rc < 0)
						{
							perror("  send() failed");
							close_conn = true;
							break;
						}*********************************/
						/* Add the new incoming connection to the     */
						/* master read set                            */
						/**********************************************/
						printf("  New incoming connection - %d\n", new_sd);
						FD_SET(new_sd, &readfds);
						if (new_sd > max_sd)
							max_sd = new_sd;

						/**********************************************/
						/* Loop back up and accept another incoming   */
						/* connection                                 */
						/**********************************************/
					// }
				}
				/****************************************************/
				/* This is not the listening socket, therefore an   */
				/* existing connection must be readable             */
				/****************************************************/
				else
				{
					printf("  Descriptor %d is readable\n", i);
					close_conn = false;
				    if (FD_ISSET(i, &tmp_readfds))
    				{
						/*************************************************/
						/* Receive all incoming data on this socket      */
						/* before we loop back and call select again.    */
						/*************************************************/
						while (rc == 3000)
						{
							/**********************************************/
							/* Receive data on this connection until the  */
							/* recv fails with EWOULDBLOCK.  If any other */
							/* failure occurs, we will close the          */
							/* connection.                                */
							/**********************************************/
							rc = recv(i, buffer, sizeof(buffer), 0);
							if (rc < 0)
							{
								if (errno != EWOULDBLOCK)
								{
								perror("  recv() failed");
								close_conn = true;
								}
								break;
							}

							/**********************************************/
							/* Check to see if the connection has been    */
							/* closed by the client                       */
							/**********************************************/
							if (rc == 0)
							{
								printf("  Connection closed\n");
								close_conn = true;
								break;
							}
						}
						/**********************************************/
						/* Data was received                          */
						/**********************************************/
						len = rc;
						printf("  %d bytes received\n", len);
						FD_CLR(i, &readfds);
						FD_SET(i, &writefds);
					}
				    else if (FD_ISSET(i, &tmp_writefds))
    				{
							/**********************************************/
							/* Echo the data back to the client           */
							/**********************************************/

						std::ifstream file("./index.html");
						std::stringstream htmlResponse;
						htmlResponse << file.rdbuf();
						file.close();
						std::string content = htmlResponse.str();
						rc = send(i, content.c_str(), content.size(), 0);
						// rc = send(i, buffer, len, 0);
						if (rc < 0)
						{
							perror("  send() failed");
							close_conn = true;
							break;
						}
						FD_CLR(i, &writefds);
						FD_SET(i, &readfds);
					}

					/*************************************************/
					/* If the close_conn flag was turned on, we need */
					/* to clean up this active connection.  This     */
					/* clean up process includes removing the        */
					/* descriptor from the master set and            */
					/* determining the new maximum descriptor value  */
					/* based on the bits that are still turned on in */
					/* the master set.                               */
					/*************************************************/
					if (close_conn)
					{
						close(i);
						FD_CLR(i, &readfds);
						if (i == max_sd)
						{
							while (FD_ISSET(max_sd, &readfds) == false)
							max_sd -= 1;
						}
					}
				} /* End of existing connection is readable */
			// } /* End of if (FD_ISSET(i, &working_set)) */
		} /* End of loop through selectable descriptors */

        // std::cout << "Waiting for connection..." << std::endl;
        // if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1)
		// {
        //     perror("accept");
        //     return(1);
        // }

        // // char buffer[30000] = {0};
        // // read(new_socket, buffer, 30000);
        // // std::cout << buffer << std::endl;
		
		// std::ifstream file("./index.html");
		// std::stringstream htmlResponse;
    	// htmlResponse << file.rdbuf();
		// file.close();
    	// // return sstr.str();

		// std::string content = htmlResponse.str();
        // // std::string htmlResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>\n";
        // send(new_socket, content.c_str(), content.size(), 0);

        // close(new_socket);
    }

   /*************************************************************/
   /* Clean up all of the sockets that are open                 */
   /*************************************************************/
   for (int i=0; i <= max_sd; ++i)
   {
      if (FD_ISSET(i, &readfds))
         close(i);
   }

    return 0;
}

# IPv4 to IPv6 Proxy Server

## Overview

This project is a proxy server written in **C** that:
- Listens for IPv4 client connections on **port 22116**.
- Responds to specific commands (e.g., `02#`) by connecting to an IPv6 server, fetching content (e.g., an HTML page), and:
  - Sending it back to the IPv4 client.
  - Saving it to a local file (`site.html`).
  - Displaying it in the server's terminal.
- Supports multiple clients simultaneously using the `fork()` system call.

---

## Features

- **Multi-Client Handling**: Manages multiple IPv4 clients concurrently using separate child processes.
- **IPv4 and IPv6 Support**: Uses sockets for both protocols to communicate between clients and servers.
- **HTTP Protocol Handling**: Sends HTTP GET requests and processes responses, including handling redirects (`HTTP 301`).
- **Error Management**: Provides meaningful error messages and ensures cleanup of resources.
- **File Logging**: Saves the fetched content to a file (`site.html`).

---

## Getting Started

### Prerequisites

Ensure you have:
- A machine with IPv6 connectivity.
- A C compiler (e.g., GCC).
- Tools like `telnet` or a custom client for testing connections.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your_username/ipv4-to-ipv6-proxy-server.git
   cd ipv4-to-ipv6-proxy-server
   ```

2. **Compile the Project**:
   ```bash
   gcc -o server main.c client.c
   ```

3. **Run the Server**:
   ```bash
   ./server
   ```

---

## Usage

1. **Start the server**:
   Run the compiled `server` program.

2. **Connect a client**:
   Use `telnet` or any network client to connect to the server:
   ```bash
   telnet <server_ip> 22116
   ```

3. **Send commands**:
   - **`02#`**: Triggers the server to connect to an IPv6 server and fetch content.
   - Any other command will result in a "Command not implemented" response.

---

## Code Structure

- **`main.c`**: Implements the IPv4 server that listens for client connections and handles commands.
- **`client.c`**: Implements the client functionality to connect to an IPv6 server and fetch content.
- **`client.h`**: Header file for `client.c`.

---

## Example Workflow

1. Start the server:
   ```bash
   ./server
   ```

2. Connect using `telnet`:
   ```bash
   telnet 127.0.0.1 22116
   ```

3. Send `02#` to fetch content from the IPv6 server.

4. The fetched content:
   - Is displayed in the server terminal.
   - Is sent back to the client.
   - Is saved locally in `site.html`.

---

## Features Under the Hood

- **Sockets**:
  - IPv4: Handles client connections.
  - IPv6: Fetches data from external servers.
- **Concurrency**:
  - Uses `fork()` to handle multiple clients.
- **HTTP Processing**:
  - Handles GET requests.
  - Follows redirects via the `Location` header.

---

## Future Improvements

- Support for HTTPS using OpenSSL.
- Advanced error handling for specific HTTP status codes.
- Integration with a front-end for monitoring clients.

---

## Contributions

Contributions are welcome! Feel free to fork this repository and submit a pull request. For major changes, please open an issue first to discuss your ideas.

---

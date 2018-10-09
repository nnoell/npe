// STL
#include <iostream>

// NPE SERVER private
#include "server.hpp"

namespace npe {
namespace server {

// The namespaced main function
bool Main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  std::cout << "NPE SERVER " << NPE_VERSION << std::endl;

  // Create and run the server
  try {
    // Create the server
    std::cout << ">> Waiting for client..." << std::endl;
    Server server(NPE_PIPE_FILE_DEFAULT_NAME, NPE_SERVER_SERVER_REQUEST_MAX_SIZE);

    // Run the server
    server.Run();

    // Exit
    std::cout << ">> Exiting..." << std::endl;
    return true;
  } catch (std::runtime_error e) {
    std::cout << ">> Server error: " << e.what() << std::endl;
    return false;
  } catch (...) {
    std::cout << ">> Server error" << std::endl;
    return false;
  }
}

}  // namespace server
}  // namespace npe

// The main function
int main(int argc, char* argv[]) {
  return !npe::server::Main(argc, argv);
}

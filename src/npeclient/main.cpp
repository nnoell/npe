// STL
#include <iostream>

// NPE PIPE public
#include <npe/pipe/request.hpp>
#include <npe/pipe/response.hpp>

// NPE CLIENT private
#include "client.hpp"

namespace npe {
namespace client {

// The namespaced main function
bool Main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  std::cout << "NPE CLIENT " << NPE_VERSION << std::endl;

  try {
    // Create the client
    Client client(NPE_PIPE_FILE_DEFAULT_NAME, NPE_CLIENT_CLIENT_REQUEST_MAX_SIZE);

    // Run the client
    client.Run();
    
    // Exit
    std::cout << ">> Exiting..." << std::endl;
    return true;
  } catch (std::runtime_error e) {
    std::cout << ">> Client error: " << e.what() << std::endl;
    return false;
  } catch (...) {
    std::cout << ">> Client error" << std::endl;
    return false;
  }
}
}  // namespace client
}  // namespace npe

// The main function
int main(int argc, char* argv[]) {
  return !npe::client::Main(argc, argv);
}

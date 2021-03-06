#include "log4cpp/Category.hh"
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include "omicron_api.h"
#include <string>

OmicronAPI::OmicronAPI(std::string address, int port, std::string username, std::string password) {
  address_ = address;
  port_ = port;
  username_ = username;
  password_ = password;
}

std::string OmicronAPI::ToString() {
  return address_ + ":" + std::to_string(port_);
}

bool OmicronAPI::CheckStatus() {

  using namespace curlpp::options;
  log4cpp::Category &root = log4cpp::Category::getRoot();
  std::stringstream results;

  root.info("Running api request");
  try {
    curlpp::Cleanup curl_cleanup;
    curlpp::Easy curl_request;
    curl_request.setOpt<Url>("http://" + this->ToString() + "/.status");
    curl_request.setOpt(new curlpp::options::UserPwd(username_ + ":" + password_));
    curl_request.setOpt(new curlpp::options::WriteStream(&results));
    curl_request.perform();
  } catch (curlpp::RuntimeError &e) {
    root.error(e.what());
    return false;
  } catch (curlpp::LogicError &e) {
    root.error(e.what());
    return false;
  }
  root.debug(results.str());

  return true;
}

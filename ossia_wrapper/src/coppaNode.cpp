#include "coppaNode.hpp"

#include "coppaDevice.hpp"
#include "coppaAddress.hpp"
#include <iostream>
coppa::ow::Node::Node(
        std::shared_ptr<coppa::ow::Node> parent,
        std::string dest):
    m_parent{parent},
    m_fullDestination{dest}
{
    std::vector<std::string> vec;
    boost::split(vec, m_fullDestination, boost::lambda::_1 == '/');

    m_name = vec.back();
}

std::shared_ptr<OSSIA::Device> coppa::ow::Node::getDevice() const
{
    return m_device;
}

std::shared_ptr<OSSIA::Node> coppa::ow::Node::getParent() const
{
    return m_parent;
}

std::string coppa::ow::Node::getName() const
{
    return m_name;
}

OSSIA::Node& coppa::ow::Node::setName(std::string)
{
    return *this;
}

std::shared_ptr<OSSIA::Address> coppa::ow::Node::getAddress() const
{
    return m_address;
}

std::shared_ptr<OSSIA::Address> coppa::ow::Node::createAddress(OSSIA::Value::Type t)
{
    // We don't change the address for now.
    if(!m_address)
        m_address = std::make_shared<coppa::ow::Address>(this->shared_from_this());
    return m_address;
}

bool coppa::ow::Node::removeAddress()
{
    return false;
}

OSSIA::Container<OSSIA::Node>::iterator coppa::ow::Node::emplace(
        OSSIA::Container<OSSIA::Node>::const_iterator,
        std::string)
{
    return {};
}

OSSIA::Container<OSSIA::Node>::iterator coppa::ow::Node::insert(
        OSSIA::Container<OSSIA::Node>::const_iterator,
        std::shared_ptr<OSSIA::Node>,
        std::string)
{
    return {};
}

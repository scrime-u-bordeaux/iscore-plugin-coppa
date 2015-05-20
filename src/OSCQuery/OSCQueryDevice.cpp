#include "OSCQueryDevice.hpp"

#include "OSCQuerySpecificSettings.hpp"

#include <coppa/oscquery/device/remote.hpp>

OSCQueryDevice::OSCQueryDevice(const DeviceSettings &settings):
    DeviceInterface{settings}
{
    m_dev = new coppa::oscquery::RemoteDevice(settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>().host.toStdString());
}

bool OSCQueryDevice::canRefresh() const
{
    return true;
}

void setNodeParameters(const coppa::oscquery::Parameter& p, QString name, Node* node)
{
    AddressSettings s;
    s.name = name;
    if(p.values.size() > 0)
    {
        switch(p.values.front().which())
        {
            case 0:
                s.valueType = "Int";
                break;

            case 1:
                s.valueType = "Float";
                break;

            case 3:
                break;

            default:
                break;
        }
    }
    node->setAddressSettings(s);
}


void insertNode(const coppa::oscquery::Parameter& p, Node* rootNode)
{
    QStringList splitted = QString::fromStdString(p.destination).split("/");
    if(splitted.first().isEmpty())
        splitted.removeFirst();
    if(splitted.size() == 1) // It's the root node
    {
        setNodeParameters(p, rootNode->name(), rootNode);
        return;
    }

    Node* currentNode = rootNode;
    for(int i = 0; i < splitted.size(); i++)
    {
        auto cdrn = currentNode->children();
        auto beg = cdrn.begin();
        auto end = cdrn.end();
        auto it = std::find_if(
                      beg,
                      end,
                      [&] (Node* n) { return n->name() == splitted[i]; });

        if(it != end)
        {
            currentNode = *it;
        }
        else
        {
            auto n = new Node;
            n->setName(splitted[i]);
            currentNode->addChild(n);
            currentNode = n;
        }
    }

    setNodeParameters(p, splitted.last(), currentNode);
}
#include <iostream>
using namespace std;
Node OSCQueryDevice::refresh()
{
    m_dev->queryNamespace();
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    const auto& theMap = m_dev->safeMap();

    Node device;
    // Print the real parameters in the tree
    for(auto&& elt : theMap.unsafeMap())
    {
        insertNode(elt, &device);
        // Get a string list and create the nodes
    }

    device.setName(settings().name);


    return device;
}


void OSCQueryDevice::addAddress(const FullAddressSettings &settings)
{
}


void OSCQueryDevice::updateAddress(const FullAddressSettings &settings)
{
}


void OSCQueryDevice::removeAddress(const QString &address)
{
}


void OSCQueryDevice::sendMessage(Message &mess)
{
    const auto& p = m_dev->get(mess.address.toStdString());

    coppa::oscquery::Values v;

    // TODO add QPointF to the supported values.
    // TODO check for a mismatch
    if(p.values.front().which() == 0)
    {
        v.values.push_back(mess.value.toInt());
    }
    else if(p.values.front().which() == 1)
    {
        v.values.push_back(mess.value.toFloat());
    }
    else if(p.values.front().which() == 3)
    {
        v.values.push_back(mess.value.toString().toStdString());
    }

    m_dev->set(mess.address.toStdString(), v);
}


bool OSCQueryDevice::check(const QString &str)
{
    qDebug() << Q_FUNC_INFO << "TODO";
    return false;
}

#ifndef __EVENT_EMITTER_H__
#define __EVENT_EMITTER_H__

#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>

template<
    class EventType = std::string,
    class Listener = std::function< void() >
>
class EventEmitter
{
public:

using Once = bool;
using EventEmitterT = EventEmitter< EventType, Listener >;
using Listeners = std::list< Listener >;
using Range = std::pair< typename Listeners::iterator, typename Listeners::iterator >;
using ListenersMap = std::unordered_map< EventType, Listeners >;
using OnceMap = std::unordered_set< const Listener * >;

public:

    EventEmitter( const EventEmitter & ) = delete;
    EventEmitter & operator= ( const EventEmitter & ) = delete;

    EventEmitter() {}

    EventEmitter( EventEmitterT && ee )
    :
    _map( std::move( ee._map ) ),
    _onceMap( std::move( ee._onceMap) ),
    _countListeners( ee._countListeners )
    {}

    EventEmitterT & on( const EventType & event, const Listener & listener )
    {
        _map[ event ].push_back( listener );
        ++_countListeners;
        return *this;
    }

    EventEmitterT & once( const EventType & event, const Listener & listener )
    {
        auto & listeners = _map[ event ];
        listeners.push_back( listener );
        ++_countListeners;
        _onceMap.insert( &listeners.back() );

        return *this;
    }

    template<class ...Args>
    void emit( const EventType & event, Args ... args )
    {
        auto it = _map.find( event );
        if( it != _map.end() )
        {
            auto & listeners = it->second;
            auto itListeners = listeners.begin();
            auto itEndListeners = listeners.end();

            while( itListeners != itEndListeners )
            {
                auto & listener = *itListeners;
                listener( std::forward<Args>(args)... );

                if( _onceMap.count( &listener ) )
                {
                    itListeners = listeners.erase( itListeners );
                    --_countListeners;
                    _onceMap.erase( &listener );
                }
                else
                {
                    ++itListeners;
                }
            }
        }
    }

    void off()
    {
        _map.clear();
        _onceMap.clear();
        _countListeners = 0;
    }

    void off( const EventType & event )
    {
        auto it = _map.find( event );
        if( it != _map.end() )
        {
            auto s = it->second.size();
            _countListeners -= s;
            _map.erase( it );

            if( _map.empty() )
            {
                _onceMap.clear();
            }
        }
    }

    Range listeners( const EventType & event )
    {
        auto it = _map.find( event );
        if( it == _map.end() )
        {
            return Range{};
        }

        return Range( it->second.begin(), it->second.end() );
    }

    Listener * listener( const EventType & event )
    {
        auto it = _map.find( event );
        if( it != _map.end() )
        {
            auto & listeners = it->second;
            return &listeners.front();
        }

        return nullptr;
    }

    std::size_t countListeners() const
    {
        return _countListeners;
    }

private:

    ListenersMap _map;
    OnceMap _onceMap;
    std::size_t _countListeners = 0;
};

#endif

#include "event_emitter.hpp"

#include <iostream>
#include <string>
#include <functional>
#include <cassert>

using ListenerT = std::function< void( const std::string & str) >;
using EventT = std::string;

int main()
{

    EventEmitter< EventT, ListenerT > emitter;

    emitter
        .on( "hello", []( const std::string & arg )
                {
                    std::cout << "Hello " << arg << std::endl;
                }
           )
        .once( "buy", []( const std::string & arg )
                {
                    std::cout << "Buy " << arg << std::endl;
                }
           )
        .on( "echo", []( const std::string & str )
                        {
                            std::cout << str << std::endl;
                        }
            );


    emitter.emit( "hello", "world" );
    emitter.emit( "echo", "echo" );

    assert( emitter.countListeners() == 3 );

    emitter.emit( "buy", "world" );

    assert( emitter.countListeners() == 2 );

    auto listener = emitter.listener( "hello" );

    assert( listener != nullptr );

    assert( emitter.remove( *listener ) );

    assert( emitter.countListeners() == 1 );

    emitter.off( "echo" );

    assert( emitter.countListeners() == 0 );
    return 0;
}

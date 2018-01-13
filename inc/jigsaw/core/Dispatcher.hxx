/********************************************************************
Copyright (c) by Ainck

Usage:

class MyAction : public IAction
{
}

class MyReceiver : public IReceiver
{
public:
    void Receive(IAction* _action) override
    {
        auto action = dynamic_cast<MyAction*>(_action);
        //do something
    }
}

class MyClass
{
public:
    void HandleAction(IAction* _Action)
    {
        auto action = dynamic_cast<MyAction*>(_action);
        //do something
    }
}

// case 1
Dispatcher::Subscribe<Receiver, Action>();

// case 2
MyClass* myClass = new MyClass();
Dispatcher::Handle<MyClass, Action>(myclass, &MyClass::HandleAction);

// case 3
Dispatcher::Handle<Action>([](IAction* _action){
    //do something
});

// create and invoke a action
NEW_ACTION(Action, action)
action->message = "notify";
INVOKE(Action, action)

*********************************************************************/

#pragma once

#include <assert.h>
#include <string.h>
#include <list>
#include <functional>
namespace jigsaw
{
	namespace core
	{
        class UUIDPrivate
        {
        public:
            static std::string generateGUID()
            {
                char buf[37];
                const char *c = "89ab";
                char *p = buf;
                int n;
                    for( n = 0; n < 16; ++n )
                    {
                        int b = rand()%255;
                        switch( n )
                        {
                            case 6:
                                sprintf(p, "4%x", b%15 );
                            break;
                            case 8:
                                sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
                            break;
                            default:
                                sprintf(p, "%02x", b);
                            break;
                        }
                        p += 2;
                        switch( n )
                        {
                            case 3:
                            case 5:
                            case 7:
                            case 9:
                                *p++ = '-';
                                break;
                        }
                    }
                    *p = 0;
                    return std::string(buf);
            }
        };

		class IAction;

		template<class _TReceiver>
		class ReceiverCreatorPrivate
		{
			friend class Dispatcher;
		public:
		private:
			static void* create()
			{
				return static_cast<void*>(new _TReceiver());
			}
		};

		class ReceiverInfoPrivate
		{
			friend class Dispatcher;
		public:
		private:
			void* (*createObject)();
		};

		class HandlerPrivate
		{
		public:
            std::string uuid;
			std::function<void(IAction*)> handle;
		};

		template< class _TAction >
		class ActionInfoPrivate
		{
			friend class Dispatcher;
		public:
		private:

            static std::list< HandlerPrivate > handlers;
            static std::list<ReceiverInfoPrivate> receivers;
		};

		template< class _TAction >
        std::list< HandlerPrivate > ActionInfoPrivate<_TAction>::handlers;

		template< class _TAction >
        std::list<ReceiverInfoPrivate> ActionInfoPrivate<_TAction>::receivers;

		class IAction
		{
		public:
            virtual ~IAction() {}
		};

		class IReceiver
		{
		public:
            virtual ~IReceiver(){}
			virtual void Receive(IAction* _action) = 0;
		};

		class Dispatcher
		{
		public:
			template<class _TAction>
			static _TAction* NewAction()
			{
				return new _TAction();
			}

			/// \Note only support Receiver:Action is N:1
			template<class _TReceiver, class _TAction>
			static void Subscribe()
			{
				ReceiverInfoPrivate receiverInfo;
				receiverInfo.createObject = ReceiverCreatorPrivate<_TReceiver>::create;
                ActionInfoPrivate<_TAction>::receivers.push_back(receiverInfo);
			}

			/// \Note only support Handler:Action is N:1
			template< class _TAction, class _TMethod>
            static std::string Handle(_TMethod _method)
			{
				HandlerPrivate handler;
				handler.handle = _method;
                handler.uuid = UUIDPrivate::generateGUID();
                ActionInfoPrivate<_TAction>::handlers.push_back(handler);
                return handler.uuid;
			}

			/// \Note only support Handler:Action is N:1
			template< class _THandler, class _TAction, class _TMethod>
            static std::string Handle(_THandler* _obj, _TMethod _method)
			{
				HandlerPrivate handler;
                handler.uuid = UUIDPrivate::generateGUID();
				handler.handle = std::bind(_method, _obj, std::placeholders::_1);
                ActionInfoPrivate<_TAction>::handlers.push_back(handler);
                return handler.uuid;
            }

            template< class _TAction>
            static void Unhandle(std::string _uuid)
            {
                auto find = [=](HandlerPrivate& o)->bool{
                    return 0 == o.uuid.compare(_uuid);
                };
                ActionInfoPrivate<_TAction>::handlers.remove_if(find);
            }

            template<class _TAction>
            static void Unhandle()
            {
                ActionInfoPrivate<_TAction>::handlers.clear();
            }

			template<class _TAction>
			static void Invoke(_TAction*& _action)
			{
				assert(_action);
				{
					auto itr = ActionInfoPrivate<_TAction>::receivers.cbegin();
					for (; itr != ActionInfoPrivate<_TAction>::receivers.cend(); ++itr)
					{
						IReceiver* receiver = static_cast<IReceiver*>(itr->createObject());
						receiver->Receive(_action);
						if (NULL != receiver)
						{
							delete receiver;
							receiver = NULL;
						}
					}
				}

				{
					auto itr = ActionInfoPrivate<_TAction>::handlers.cbegin();
					for (; itr != ActionInfoPrivate<_TAction>::handlers.cend(); ++itr)
					{
						itr->handle(_action);
					}
				}

				if (NULL != _action)
				{
					delete _action;
					_action = NULL;
				}
            }
		private:

		};//class Dispatcher
	} //namespace core
} //namespace jigsaw

#define NEW_ACTION(classname, variable) classname* variable = jigsaw::core::Dispatcher::NewAction<classname>();
#define INVOKE_ACTION(classname, variable) jigsaw::core::Dispatcher::Invoke<classname>(variable);

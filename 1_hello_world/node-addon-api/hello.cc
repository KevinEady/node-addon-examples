#include <napi.h>

void doWork(std::function<void()> cb) { cb(); }

class AddonInstance : public Napi::ObjectWrap<AddonInstance> {
public:
  AddonInstance(const Napi::CallbackInfo &info)
      : Napi::ObjectWrap<AddonInstance>(info) {}

  static Napi::Function GetClass(Napi::Env env) {
    return DefineClass(
        env, "AddonInstance",
        {
            AddonInstance::InstanceMethod("exportedFunction",
                                          &AddonInstance::ExportedFunction),
        });
  }

  Napi::Value ExportedFunction(const Napi::CallbackInfo &info) {

    _callbackRef = Napi::Persistent(info[0].As<Napi::Function>());

    doWork([this] {
      this->_callbackRef.Value().Call(
          {Napi::String::New(this->_callbackRef.Env(), "cpp-arg0"),
           Napi::String::New(this->_callbackRef.Env(), "cpp-arg1")});
    });
    return info.Env().Undefined();
  }

private:
  Napi::FunctionReference _callbackRef;
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "AddonInstance"),
              AddonInstance::GetClass(env));
  return exports;
}

NODE_API_MODULE(hello, Init)

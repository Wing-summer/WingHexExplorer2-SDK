#ifndef IWINGANGEL_H
#define IWINGANGEL_H

#include "WingPlugin/wingplugin_global.h"

namespace WingHex {

// some classes and helper copied from angelscript.h

class asIUnknownClass;
typedef void (asIUnknownClass::*asIMETHOD_t)();
typedef void (*asIFUNCTION_t)();

struct asFuncPtr {
    asFuncPtr(uchar f = 0) {
        for (size_t n = 0; n < sizeof(ptr.dummy); n++)
            ptr.dummy[n] = 0;
        flag = f;
    }

    void CopyMethodPtr(const void *mthdPtr, size_t size) {
        for (size_t n = 0; n < size; n++)
            ptr.dummy[n] = reinterpret_cast<const char *>(mthdPtr)[n];
    }

    union {
        // The largest known method point is 20 bytes (MSVC 64bit),
        // but with 8byte alignment this becomes 24 bytes. So we need
        // to be able to store at least that much.
        char dummy[25];
        struct {
            asIMETHOD_t mthd;
            char dummy[25 - sizeof(asIMETHOD_t)];
        } m;
        struct {
            asIFUNCTION_t func;
            char dummy[25 - sizeof(asIFUNCTION_t)];
        } f;
    } ptr;

    uchar flag; // 1 = generic, 2 = global func, 3 = method
};

// Method pointers

// Declare a dummy class so that we can determine the size of a simple
// method pointer
class asCSimpleDummy {};
typedef void (asCSimpleDummy::*asSIMPLEMETHOD_t)();
static constexpr int SINGLE_PTR_SIZE = sizeof(asSIMPLEMETHOD_t);

// Define template
template <int N>
struct asMethodPtr {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // This version of the function should never be executed, nor
        // compiled, as it would mean that the size of the method pointer
        // cannot be determined.

        int ERROR_UnsupportedMethodPtr[N - 100];

        asFuncPtr p(0);
        return p;
    }
};

// Template specialization
template <>
struct asMethodPtr<SINGLE_PTR_SIZE> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE);
        return p;
    }
};

#if defined(_MSC_VER) && !defined(__MWERKS__)
// MSVC and Intel uses different sizes for different class method pointers
template <>
struct asMethodPtr<SINGLE_PTR_SIZE + 1 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE + sizeof(int));
        return p;
    }
};

template <>
struct asMethodPtr<SINGLE_PTR_SIZE + 2 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // On 32bit platforms with is where a class with virtual inheritance
        // falls. On 64bit platforms we can also fall here if 8byte data
        // alignments is used.

        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE + 2 * sizeof(int));

        // Microsoft has a terrible optimization on class methods with
        // virtual inheritance. They are hardcoding an important offset,
        // which is not coming in the method pointer.

#if defined(_MSC_VER) && !defined(AS_64BIT_PTR)
        // Method pointers for virtual inheritance is not supported,
        // as it requires the location of the vbase table, which is
        // only available to the C++ compiler, but not in the method
        // pointer.

        // You can get around this by forward declaring the class and
        // storing the sizeof its method pointer in a constant. Example:

        // class ClassWithVirtualInheritance;
        // const int ClassWithVirtualInheritance_workaround = sizeof(void
        // ClassWithVirtualInheritance::*());

        // This will force the compiler to use the unknown type
        // for the class, which falls under the next case

        // Copy the virtual table index to the 4th dword so that AngelScript
        // can properly detect and deny the use of methods with virtual
        // inheritance.
        *(reinterpret_cast<quint32 *>(&p) + 3) =
            *(reinterpret_cast<quint32 *>(&p) + 2);
#endif

        return p;
    }
};

template <>
struct asMethodPtr<SINGLE_PTR_SIZE + 3 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE + 3 * sizeof(int));
        return p;
    }
};

template <>
struct asMethodPtr<SINGLE_PTR_SIZE + 4 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // On 64bit platforms with 8byte data alignment
        // the unknown class method pointers will come here.

        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE + 4 * sizeof(int));
        return p;
    }
};

#endif

#define AS_IMETHOD_AMBIGUITY_CAST(t) static_cast<t>

#define asIMETHOD(c, m)                                                        \
    asMethodPtr<sizeof(void(c::*)())>::Convert((void(c::*)())(&c::m))
#define asIMETHODPR(c, m, p, r)                                                \
    asMethodPtr<sizeof(void(c::*)())>::Convert(                                \
        AS_IMETHOD_AMBIGUITY_CAST(r(c::*) p)(&c::m))

/**
 * @brief The IWingAngel class
 * @note we can register the script types by this class
 */
class WINGPLUGIN_EXPORT IWingAngel {
public:
    IWingAngel();

public:
    // copy from angelscript.h
    enum asBehaviours {
        // Value object memory management
        asBEHAVE_CONSTRUCT,
        asBEHAVE_LIST_CONSTRUCT,
        asBEHAVE_DESTRUCT,

        // Reference object memory management
        asBEHAVE_FACTORY,
        asBEHAVE_LIST_FACTORY,
        asBEHAVE_ADDREF,
        asBEHAVE_RELEASE,
        asBEHAVE_GET_WEAKREF_FLAG,

        // Object operators
        asBEHAVE_TEMPLATE_CALLBACK,

        // Garbage collection behaviours
        asBEHAVE_FIRST_GC,
        asBEHAVE_GETREFCOUNT = asBEHAVE_FIRST_GC,
        asBEHAVE_SETGCFLAG,
        asBEHAVE_GETGCFLAG,
        asBEHAVE_ENUMREFS,
        asBEHAVE_RELEASEREFS,
        asBEHAVE_LAST_GC = asBEHAVE_RELEASEREFS,

        asBEHAVE_MAX
    };

    // copy from angelscript.h
    enum class asRetCodes {
        asSUCCESS = 0,
        asERROR = -1,
        asCONTEXT_ACTIVE = -2,
        asCONTEXT_NOT_FINISHED = -3,
        asCONTEXT_NOT_PREPARED = -4,
        asINVALID_ARG = -5,
        asNO_FUNCTION = -6,
        asNOT_SUPPORTED = -7,
        asINVALID_NAME = -8,
        asNAME_TAKEN = -9,
        asINVALID_DECLARATION = -10,
        asINVALID_OBJECT = -11,
        asINVALID_TYPE = -12,
        asALREADY_REGISTERED = -13,
        asMULTIPLE_FUNCTIONS = -14,
        asNO_MODULE = -15,
        asNO_GLOBAL_VAR = -16,
        asINVALID_CONFIGURATION = -17,
        asINVALID_INTERFACE = -18,
        asCANT_BIND_ALL_FUNCTIONS = -19,
        asLOWER_ARRAY_DIMENSION_NOT_REGISTERED = -20,
        asWRONG_CONFIG_GROUP = -21,
        asCONFIG_GROUP_IS_IN_USE = -22,
        asILLEGAL_BEHAVIOUR_FOR_TYPE = -23,
        asWRONG_CALLING_CONV = -24,
        asBUILD_IN_PROGRESS = -25,
        asINIT_GLOBAL_VARS_FAILED = -26,
        asOUT_OF_MEMORY = -27,
        asMODULE_IS_IN_USE = -28
    };

    // ============= low level functions begin =============
public:
    virtual asRetCodes setDefaultNamespace(const char *nameSpace) = 0;
    virtual asRetCodes restoreDefaultNamespace() = 0;

public:
    virtual asRetCodes registerEnum(const char *type) = 0;
    virtual asRetCodes registerEnumValue(const char *type, const char *name,
                                         int value) = 0;

public:
    virtual asRetCodes registerFuncdef(const char *decl) = 0;

public:
    virtual asRetCodes registerTypedef(const char *type, const char *decl) = 0;

public:
    virtual asRetCodes registerGlobalProperty(const char *declaration,
                                              void *pointer) = 0;

public:
    virtual asRetCodes registerObjectType(const char *obj, int byteSize,
                                          quint64 flags) = 0;

    virtual asRetCodes
    registerObjectProperty(const char *obj, const char *declaration,
                           int byteOffset, int compositeOffset = 0,
                           bool isCompositeIndirect = false) = 0;
    virtual asRetCodes
    registerObjectMethod(const char *obj, const char *declaration,
                         const asFuncPtr &funcPointer, void *auxiliary = 0,
                         int compositeOffset = 0,
                         bool isCompositeIndirect = false) = 0;

    virtual asRetCodes registerObjectBehaviour(
        const char *obj, asBehaviours behaviour, const char *declaration,
        const asFuncPtr &funcPointer, void *auxiliary = 0,
        int compositeOffset = 0, bool isCompositeIndirect = false) = 0;

public:
    virtual asRetCodes registerInterface(const char *name) = 0;

    virtual asRetCodes registerInterfaceMethod(const char *intf,
                                               const char *declaration) = 0;

public:
    virtual asRetCodes registerGlobalFunction(const char *declaration,
                                              const asFuncPtr &funcPointer,
                                              void *auxiliary = 0) = 0;

public:
    asRetCodes registerEnums(const char *type,
                             const QHash<const char *, int> value);
};

} // namespace WingHex

#endif // IWINGANGEL_H

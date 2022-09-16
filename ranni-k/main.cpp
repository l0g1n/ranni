#include <ntifs.h>
#include <ntddk.h>

DRIVER_UNLOAD DriverUnload;
_Dispatch_type_(IRP_MJ_CREATE) DRIVER_DISPATCH DeviceCreate;
_Dispatch_type_(IRP_MJ_CLOSE) DRIVER_DISPATCH DeviceClose;
_Dispatch_type_(IRP_MJ_CLEANUP) DRIVER_DISPATCH DeviceCleanup;
_Dispatch_type_(IRP_MJ_DEVICE_CONTROL) DRIVER_DISPATCH DeviceControl;

extern "C"
{
    NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
    {
        UNREFERENCED_PARAMETER(DriverObject);
        UNREFERENCED_PARAMETER(RegistryPath);

        __debugbreak();

        DriverObject->DriverUnload                         = DriverUnload;
        DriverObject->MajorFunction[IRP_MJ_CREATE]         = DeviceCreate;
        DriverObject->MajorFunction[IRP_MJ_CLOSE]          = DeviceClose;
        DriverObject->MajorFunction[IRP_MJ_CLEANUP]        = DeviceCleanup;
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControl;

        NTSTATUS Status;

        UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\ranni");
        PDEVICE_OBJECT DeviceObject;
        Status = IoCreateDevice(
            DriverObject,
            0,
            &DeviceName,
            FILE_DEVICE_UNKNOWN,
            0,
            FALSE,
            &DeviceObject
        );
        if (!NT_SUCCESS(Status)) {
            KdPrint(("IoCreateDevice failed, error code: 0x%x", Status));
            return Status;
        }

        UNICODE_STRING SymbolicLink = RTL_CONSTANT_STRING(L"\\??\\ranni");
        Status                      = IoCreateSymbolicLink(&SymbolicLink, &DeviceName);
        if (!NT_SUCCESS(Status)) {
            KdPrint(("IoCreateSymbolicLink failed, error code: 0x%x", Status));
            IoDeleteDevice(DeviceObject);
            return Status;
        }

        return Status;
    }
}

void DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING SymbolicLink = RTL_CONSTANT_STRING(L"\\??\\ranni");
    IoDeleteSymbolicLink(&SymbolicLink);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DeviceCreate(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

NTSTATUS DeviceClose(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

NTSTATUS DeviceCleanup(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

NTSTATUS DeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

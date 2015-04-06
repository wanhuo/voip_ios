// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_WIN_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_WIN_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "device/bluetooth/bluetooth_device.h"
#include "device/bluetooth/bluetooth_task_manager_win.h"

namespace device {

class BluetoothAdapterWin;
class BluetoothServiceRecordWin;
class BluetoothSocketThread;

class BluetoothDeviceWin : public BluetoothDevice {
 public:
  explicit BluetoothDeviceWin(
      const BluetoothTaskManagerWin::DeviceState& device_state,
      const scoped_refptr<base::SequencedTaskRunner>& ui_task_runner,
      const scoped_refptr<BluetoothSocketThread>& socket_thread,
      net::NetLog* net_log,
      const net::NetLog::Source& net_log_source);
  virtual ~BluetoothDeviceWin();

  // BluetoothDevice override
  virtual uint32 GetBluetoothClass() const override;
  virtual std::string GetAddress() const override;
  virtual VendorIDSource GetVendorIDSource() const override;
  virtual uint16 GetVendorID() const override;
  virtual uint16 GetProductID() const override;
  virtual uint16 GetDeviceID() const override;
  virtual int GetRSSI() const override;
  virtual int GetCurrentHostTransmitPower() const override;
  virtual int GetMaximumHostTransmitPower() const override;
  virtual bool IsPaired() const override;
  virtual bool IsConnected() const override;
  virtual bool IsConnectable() const override;
  virtual bool IsConnecting() const override;
  virtual UUIDList GetUUIDs() const override;
  virtual bool ExpectingPinCode() const override;
  virtual bool ExpectingPasskey() const override;
  virtual bool ExpectingConfirmation() const override;
  virtual void Connect(
      PairingDelegate* pairing_delegate,
      const base::Closure& callback,
      const ConnectErrorCallback& error_callback) override;
  virtual void SetPinCode(const std::string& pincode) override;
  virtual void SetPasskey(uint32 passkey) override;
  virtual void ConfirmPairing() override;
  virtual void RejectPairing() override;
  virtual void CancelPairing() override;
  virtual void Disconnect(
      const base::Closure& callback,
      const ErrorCallback& error_callback) override;
  virtual void Forget(const ErrorCallback& error_callback) override;
  virtual void ConnectToService(
      const BluetoothUUID& uuid,
      const ConnectToServiceCallback& callback,
      const ConnectToServiceErrorCallback& error_callback) override;
  virtual void ConnectToServiceInsecurely(
      const BluetoothUUID& uuid,
      const ConnectToServiceCallback& callback,
      const ConnectToServiceErrorCallback& error_callback) override;
  virtual void CreateGattConnection(
      const GattConnectionCallback& callback,
      const ConnectErrorCallback& error_callback) override;
  virtual void StartConnectionMonitor(
      const base::Closure& callback,
      const ErrorCallback& error_callback) override;

  // Used by BluetoothProfileWin to retrieve the service record for the given
  // |uuid|.
  const BluetoothServiceRecordWin* GetServiceRecord(
      const device::BluetoothUUID& uuid) const;

  // Returns true if all fields and services of this instance are equal to the
  // fields and services stored in |device_state|.
  bool IsEqual(const BluetoothTaskManagerWin::DeviceState& device_state);

  // Updates this instance with all fields and properties stored in
  // |device_state|.
  void Update(const BluetoothTaskManagerWin::DeviceState& device_state);

 protected:
  // BluetoothDevice override
  virtual std::string GetDeviceName() const override;

 private:
  friend class BluetoothAdapterWin;

  // Used by BluetoothAdapterWin to update the visible state during
  // discovery.
  void SetVisible(bool visible);

  // Updates the services with services stored in |device_state|.
  void UpdateServices(const BluetoothTaskManagerWin::DeviceState& device_state);

  scoped_refptr<base::SequencedTaskRunner> ui_task_runner_;
  scoped_refptr<BluetoothSocketThread> socket_thread_;
  net::NetLog* net_log_;
  net::NetLog::Source net_log_source_;

  // The Bluetooth class of the device, a bitmask that may be decoded using
  // https://www.bluetooth.org/Technical/AssignedNumbers/baseband.htm
  uint32 bluetooth_class_;

  // The name of the device, as supplied by the remote device.
  std::string name_;

  // The Bluetooth address of the device.
  std::string address_;

  // Tracked device state, updated by the adapter managing the lifecycle of
  // the device.
  bool paired_;
  bool connected_;

  // Used to send change notifications when a device disappears during
  // discovery.
  bool visible_;

  // The services (identified by UUIDs) that this device provides.
  UUIDList uuids_;

  // The service records retrieved from SDP.
  typedef ScopedVector<BluetoothServiceRecordWin> ServiceRecordList;
  ServiceRecordList service_record_list_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothDeviceWin);
};

}  // namespace device

#endif  // DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_WIN_H_

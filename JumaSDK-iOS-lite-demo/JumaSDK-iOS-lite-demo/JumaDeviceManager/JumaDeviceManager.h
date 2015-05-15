//
//  JumaDeviceManager.h
//  JumaDeviceManager
//
//  Created by Wang AnJun on 15/4/28.
//  Copyright (c) 2015年 Jumacc. All rights reserved.
//  Juma SDK iOS lite version release 1.0

#import <Foundation/Foundation.h> 

/** If a macro named JumaLogEnable was defined as 1, JumaDeviceManager will log the some information when there is no delegate method invoked after you called certain method */
#define JumaLogEnable 1

/*!
 *  @enum JumaDeviceManagerState
 *
 *  @discussion Represents the current state of a JumaDeviceManager.
 *
 *  @constant JumaDeviceManagerStateUnknown       State unknown, update imminent.
 *  @constant JumaDeviceManagerStateResetting     The connection with the system service was momentarily lost, update imminent.
 *  @constant JumaDeviceManagerStateUnsupported   The platform doesn't support the Bluetooth Low Energy Central/Client role.
 *  @constant JumaDeviceManagerStateUnauthorized  The application is not authorized to use the Bluetooth Low Energy Central/Client role.
 *  @constant JumaDeviceManagerStatePoweredOff    Bluetooth is currently powered off.
 *  @constant JumaDeviceManagerStatePoweredOn     Bluetooth is currently powered on and available to use.
 *
 */
typedef NS_ENUM(NSInteger, JumaDeviceManagerState) {
    JumaDeviceManagerStateUnknown = 0,
    JumaDeviceManagerStateResetting,
    JumaDeviceManagerStateUnsupported,
    JumaDeviceManagerStateUnauthorized,
    JumaDeviceManagerStatePoweredOff,
    JumaDeviceManagerStatePoweredOn,
};

@protocol JumaDeviceManagerDelegate;


@interface JumaDeviceManager : NSObject

@property (nonatomic, weak) id<JumaDeviceManagerDelegate> delegate; 

/*!
 *  @method scanDeviceWithName:
 *
 *  @param deviceName    A name representing the device to scan for.
 *
 *  @discussion          Starts scanning for devices that has a name same as the <i>deviceName</i> parameter.
 *                       If <i>deviceName</i> is <i>nil</i> all discovered devices will be returned.
 *
 */
- (void)scanDeviceWithName:(NSString *)deviceName;

/*!
 *  @method scanDeviceWithName:timeout:
 *
 *  @param deviceName    A name representing the device to scan for.
 *  @param timeLength    The length of the time for scanning.
 *
 *  @discussion          Starts scanning for devices that has a name same as the <i>deviceName</i> parameter.
 *                       If <i>deviceName</i> is <i>nil</i>, all discovered devices will be returned.
 *                       If <i>timeLength</i> is zero or nagetive, the device manager will scan continually.
 *                       If <i>timeLength</i> is positive, tthe device manager will call {@method stopScan} after <i>timeLength</i> seconds.
 *
 */
- (void)scanDeviceWithName:(NSString *)deviceName timeLength:(NSTimeInterval)timeLength;
/*!
 *  @method stopScan:
 *
 *  @discussion Stops scanning for devices.
 *
 */
- (void)stopScan;


/*!
 *  @method connectDevice:
 *
 *  @param deviceUUID   The UUID of the device to be connected.
 *
 *  @discussion         Initiates a connection to certain device. Connection attempts never time out and, depending on the outcome, will result
 *                      in a call to either {@method deviceManager:didConnectDevice:} or {@method deviceManager:didFailToConnectDevice:error:}.
 *                      Pending attempts are cancelled automatically upon deallocation of the device, and explicitly via {@method disconnectDevice}.
 */
- (void)connectDevice:(NSString *)deviceUUID;
/*!
 *  @method disconnectDevice:
 *
 *  @discussion         Cancels an active or pending connection to certain device. Note that this is non-blocking, and any device
 *                      commands that are still pending to device may or may not complete.
 *
 *  @see                deviceManager:didDisconnectDevice:byRemote:code:error:
 *
 */
- (void)disconnectDevice;



/*!
 *  @method readRSSI
 *
 *  @discussion While connected, retrieves the current RSSI of the link.
 *
 *  @see        deviceManager:didReadRSSI:error:
 */
- (void)readRSSI;


/*!
 *  @method sendData:
 *
 *  @param data				The data to send.
 *
 *  @discussion				Sends <i>data</i> to device.
 *							If the <i>data</i> was sent successfully and the device has sent a response to the device manager, there will be a call
 *                          to {@method deviceManager:didReceiveData:error:}. The maximum length of <i>data</i>  was designated as 19 in this version.
 */
- (void)sendData:(NSData *)data;

@end

@protocol JumaDeviceManagerDelegate <NSObject>

@optional

/*!
 *  @method deviceManager:didUpdateState::
 *
 *  @param deviceManager   The device manager whose state has changed.
 *  @param state           The current state of the device manager.
 *
 *  @discussion            Invoked whenever the device manager's state has been updated. Commands should only be issued when the state is
 *                         <code>JumaDeviceManagerStatePoweredOn</code>. A state below <code>JumaDeviceManagerStatePoweredOn</code>
 *                         implies that scanning has stopped and any connected peripherals have been disconnected.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didUpdateState:(JumaDeviceManagerState)state;


/*!
 *  @method deviceManager:didDiscoverDevice:name:RSSI:
 *
 *  @param deviceManager    The device manager providing this update.
 *  @param deviceUUID       The UUID of the device discovered.
 *  @param deviceName       The name        of the device discovered.
 *  @param RSSI             The current RSSI of device, in dBm. A value of <code>127</code> is reserved and indicates the RSSI
 *                          was not available.
 *
 *  @discussion             This method is invoked while scanning, upon the discovery of <i>device</i> by <i>manager</i>.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didDiscoverDevice:(NSString *)deviceUUID name:(NSString *)deviceName RSSI:(NSNumber *)RSSI;

/*!
 *  @method deviceManagerDidStopScan:
 *
 *  @param deviceManager    The device manager providing this update.
 *
 *  @discussion             This method is invoked after {@method stopScan} called.
 *
 */
- (void)deviceManagerDidStopScan:(JumaDeviceManager *)deviceManager;


/*!
 *  @method deviceManager:didConnectDevice:
 *
 *  @param deviceManager    The device manager providing this information.
 *  @param deviceUUID       The UUID of the device that has connected.
 *
 *  @discussion             This method is invoked when a connection initiated by {@method connectDevice:} has succeeded.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didConnectDevice:(NSString *)deviceUUID;
/*!
 *  @method deviceManager:didFailToConnectDevice:error:
 *
 *  @param deviceManager    The device manager providing this information.
 *  @param deviceUUID       The UUID of the device that has failed to connect.
 *  @param error            The cause of the failure.
 *
 *  @discussion             This method is invoked when a connection initiated by {@method connectDevice:} has failed to complete. 
 *                          As connection attempts do not timeout, the failure of a connection is atypical and usually indicative of a transient issue.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didFailToConnectDevice:(NSString *)deviceUUID error:(NSString *)error;
/*!
 *  @method deviceManager:didDisconnectDevice:byRemote:code:error:
 *
 *  @param deviceManager    The device manager providing this information.
 *  @param deviceUUID       The UUID of the device that has disconnected.
 *  @param byRemote         not implemented.
 *  @param code             not implemented.
 *  @param error            If an error occurred, the cause of the failure.
 *
 *  @discussion             This method is invoked upon the disconnection of a device that was connected by {@method connectDevice:}.
 *                          If the disconnection was not initiated by {@method disconnectDevice}, the cause will be detailed in the <i>error</i> parameter.
 *                          Otherwise, there is no any cause will be detailed in the <i>error</i> parameter.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didDisconnectDevice:(NSString *)deviceUUID byRemote:(BOOL)byRemote code:(NSInteger)code error:(NSString *)error;

/*!
 *  @method deviceManager:didReceiveData:error:
 *
 *  @param deviceManager    The device manager providing this information.
 *  @param deviceUUID       The data received from the device that has connected.
 *  @param error            If an error occurred, the cause of the failure.
 *
 *  @discussion             This method is invoked upon the device manager has received some data from the device that receive the data sent to it by a {@method sendData:} call.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didReceiveData:(NSData *)data error:(NSString *)error;

/*!
 *  @method deviceManager:didReadRSSI:error:
 *
 *  @param deviceManager    The device manager providing this update.
 *  @param RSSI             The current RSSI of the link.
 *  @param error            If an error occurred, the cause of the failure.
 *
 *  @discussion             This method returns the result of a {@method readRSSI} call.
 *
 */
- (void)deviceManager:(JumaDeviceManager *)deviceManager didReadRSSI:(NSNumber *)RSSI error:(NSString *)error;

@end

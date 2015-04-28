//
//  ViewController.m
//  JumaSDK-iOS-lite
//
//  Created by Wang AnJun on 15/4/28.
//  Copyright (c) 2015年 Jumacc. All rights reserved.
//

#import "ViewController.h"
#import "JumaDeviceManager.h"


@interface ViewController () <JumaDeviceManagerDelegate>

@property (nonatomic, copy) NSString *deviceUUID;
@property (nonatomic, copy) NSString *deviceName;

@property (weak, nonatomic) IBOutlet UITextView *textView;

@property (weak, nonatomic) IBOutlet UITextField *nameField;
@property (weak, nonatomic) IBOutlet UITextField *dataField;
@property (weak, nonatomic) IBOutlet UIButton *sendBtn;
@property (weak, nonatomic) IBOutlet UIButton *scanBtn;

@property (nonatomic, strong) JumaDeviceManager *deviceManager;

@end

@implementation ViewController

- (IBAction)scan:(id)sender {
    
    self.deviceName = self.nameField.text;
    [self.deviceManager scanDeviceWithName:self.deviceName];
    self.scanBtn.enabled = NO;
    
    NSLog(@"scanning");
    self.textView.text = [self.textView.text stringByAppendingString:@"scanning\n"];
}

- (IBAction)send:(UIButton *)sender {
    
    NSData *data = [self.dataField.text dataUsingEncoding:NSUTF8StringEncoding];
    [self.deviceManager sendData:data];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.sendBtn.layer.borderColor = [UIColor colorWithRed:80/255.0 green:80/255.0 blue:80/255.0 alpha:0.3].CGColor;
    self.sendBtn.layer.borderWidth = 1;
    self.sendBtn.layer.cornerRadius = 5;
    
    self.scanBtn.layer.borderColor = [UIColor colorWithRed:80/255.0 green:80/255.0 blue:80/255.0 alpha:0.3].CGColor;
    self.scanBtn.layer.borderWidth = 1;
    self.scanBtn.layer.cornerRadius = 5;
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    self.deviceManager = [[JumaDeviceManager alloc] init];
    self.deviceManager.delegate = self;
}

#pragma mark - JumaDeviceManagerDelegate

- (void)deviceManager:(JumaDeviceManager *)deviceManager didUpdateState:(JumaDeviceManagerState)state {
    
    switch (state) {
        case JumaDeviceManagerStatePoweredOn:    NSLog(@"Bluetooth is powerd on");     break;
        case JumaDeviceManagerStatePoweredOff:   NSLog(@"Bluetooth is powerd off");          return;
        case JumaDeviceManagerStateUnauthorized: NSLog(@"Bluetooth is unauthorized");        return;
        case JumaDeviceManagerStateUnsupported:  NSLog(@"Bluetooth state is unsupported");   return;
        case JumaDeviceManagerStateResetting:    NSLog(@"Bluetooth is being reset");         return;
        case JumaDeviceManagerStateUnknown:      NSLog(@"Bluetooth state is unknown");       return;
    }
    
    self.scanBtn.enabled = YES;
    
    self.textView.text = @"Bluetooth is powered on. You can scan now.\n";
}

- (void)deviceManagerDidStopScan:(JumaDeviceManager *)deviceManager {
    
    self.scanBtn.enabled = YES;
    
    self.textView.text = [self.textView.text stringByAppendingString:@"did stop scan\n"];
}

- (void)deviceManager:(JumaDeviceManager *)deviceManager didDiscoverDevice:(NSString *)deviceUUID name:(NSString *)deviceName RSSI:(NSNumber *)RSSI {
    
    if ([self.deviceName isEqualToString:deviceName] == NO) return;
    
    self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"did found device: %@\n", deviceName]];
    
    NSLog(@"did found device: %@", deviceName);
    
    //    self.deviceUUID = deviceUUID;
    [deviceManager connectDevice:deviceUUID];
}

- (void)deviceManager:(JumaDeviceManager *)deviceManager didConnectDevice:(NSString *)deviceUUID {
    
    self.sendBtn.enabled = YES;
    
    self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"did connect device: %@\n", self.deviceName]];
    
    //    NSData *data = [@"text for test" dataUsingEncoding:NSUTF8StringEncoding];
    //    [deviceManager sendData:data];
}

- (void)deviceManager:(JumaDeviceManager *)deviceManager didFailToConnectDevice:(NSString *)deviceUUID error:(NSString *)error {
    
    self.sendBtn.enabled = NO;
    
    self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"did fail to connect device: %@\n", self.deviceName]];
}

- (void)deviceManager:(JumaDeviceManager *)deviceManager didDisconnectDevice:(NSString *)deviceUUID error:(NSString *)error {
    
    self.sendBtn.enabled = NO;
    
    self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"did disconnect device: %@, error : %@\n", self.deviceName, error]];
    NSLog(@"did disconnect device, error : %@", error);
    
    [deviceManager connectDevice:deviceUUID];
}

- (void)deviceManager:(JumaDeviceManager *)deviceManager didReceiveData:(NSData *)data error:(NSString *)error {
    
    if (error) {
        self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"did recevice value from device <%@>\n", self.deviceName]];
        
        NSLog(@"did fail to get value from device, error : %@", error);
    }
    else {
        
        NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        
        self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"did recevice string <%@> from device <%@>\n", string, self.deviceName]];
        
        NSLog(@"did get value : %@", data);
    }
    
    self.dataField.text = nil;
}

@end


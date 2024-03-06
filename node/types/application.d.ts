import { Feature } from "./feature";
export type Element = Feature | {
    [key: string]: Element;
};
/**
 * 应用基本结构
 */
export interface Application {
    /** 应用名 */
    readonly name: string;
    /** 应用版本号 */
    readonly version: string;
    /** 应用元素列表 */
    readonly elements: {
        [key: string]: Element;
    };
    /** 应用默认功能点分路径 */
    readonly main?: string;
    /**
     * 应用主入口，若设置，则在调用特性之前先执行此入口
     * 若此入口返回非零，则放弃调用特性入口
     */
    readonly entry?: () => number | Promise<number>;
    /** 简介 */
    readonly brief?: string;
    /** 详细介绍 */
    readonly description?: string;
    /** 选项列表 */
    readonly options?: any;
    /** 可以指定一个标志表示选项结束，后续命令行参数全被用作功能参数 */
    readonly options_end?: string;
    /** 帮助功能入口点分路径 */
    readonly help?: string;
}
/**
 * 获取全局选项
 */
export declare function GetGlobalOptions(): any;
export declare function IsModule(obj: any): obj is {
    [key: string]: Feature;
};
export declare function IsFeature(obj: any): obj is Feature;
export declare function IsElement(obj: any): obj is Element;
export declare function IsApplication(obj: any): obj is Application;
export declare function locateElement(path: string[]): [Element, string[]];
export declare function verifyDefinitions(module: any, global_names?: Set<string>): any;
